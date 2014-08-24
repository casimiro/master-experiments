#include "evaluation.h"
#include <fstream>
#include <iostream>
#include <QSql>
#include <QSqlQuery>
#include <QVariant>

namespace casimiro {

Evaluation::Evaluation()
{
}

Evaluation::~Evaluation()
{
}

Metrics Evaluation::getMetrics(const TweetVector& _sortedCandidates, const Tweet& _retweet) const
{
    int i = 0;
    int pos = -1;
    for(auto tweet : _sortedCandidates)
    {
        if(tweet.getTweetId() == _retweet.getRetweetedId())
        {
            pos = i;
            break;
        }
        i++;
    }
    return Metrics(pos);
}

bool RetweetHasAnyTimedTopic(const StringIntMaps& _topicLifeSpanMaps, const Tweet& _retweet)
{
    if(_topicLifeSpanMaps.size() == 0)
        return true; // if there is no timed topic in the system just return true
        
    auto topicLifeSpanMap = _topicLifeSpanMaps.at(0);
    for(auto pair : _retweet.getProfile())
        if(topicLifeSpanMap.count(pair.first) > 0)
            return true;
    return false;
}

std::map<int, StringIntMap> LoadUserTopicLifeSpanMaps(const TwitterUser& _user, ProfileType _profileType)
{
    std::map<int, StringIntMap> aux;
    QSqlQuery query;
    if(_profileType == BOWProfile)
        query.prepare("SELECT word, normal_area, word_timing FROM user_word_timing WHERE user_id = :uid");
    else
        query.prepare("SELECT topic, normal_area, topic_timing FROM user_topic_timing WHERE user_id = :uid");
    query.bindValue(":uid", static_cast<qlonglong>(_user.getUserId()));
    query.exec();
    
    while(query.next())
    {
        auto topic = query.value(0).toString().toStdString();
        auto normalArea = (query.value(1).toInt() / 10) - 1;
        auto topicTiming = query.value(2).toInt();
        
        if(aux.find(normalArea) == aux.end())
            aux.insert(std::make_pair(normalArea, StringIntMap()));
        
        aux.find(normalArea)->second.insert(std::make_pair(topic, topicTiming));
    }
    
    return aux;
}

MetricsVectors Evaluation::evaluateUser(TwitterUser& _user,
                                        ProfileType _profileType,
                                        const QDateTime& _startProfile, 
                                        const QDateTime& _endProfile, 
                                        const QDateTime& _startRetweets, 
                                        const QDateTime& _endRetweets, 
                                        int _candidatePeriodInHours,
                                        const StringIntMaps& _topicLifeSpanMaps,
                                        bool _ignoreRetweetsWithoutTimedTopics,
                                        bool _usePersonalisedLifeSpanMaps)
{
    MetricsVectors metricsVectors;
    metricsVectors.push_back(MetricsVector());
    for(auto topicLifeMap : _topicLifeSpanMaps)
        metricsVectors.push_back(MetricsVector());
    
    auto userTopicLifeSpanMaps = LoadUserTopicLifeSpanMaps(_user, _profileType);
    
    if(_profileType == TopicProfile)
        _user.loadProfile(_startProfile, _endProfile);
    else if(_profileType == BOWProfile)
        _user.loadBOWProfile(_startProfile, _endProfile);
    else if(_profileType == SVMProfile)
    {
        std::stringstream ss;
        ss << "data/models/" << _user.getUserId() << "_model.txt";
        _user.loadSVMProfile(ss.str());
    }
    
    auto retweets = _user.getRetweets(_startRetweets, _endRetweets);
    for(auto retweet : retweets)
    {
        // discards retweets that have no topic with life cycle.
        // this happens when evaluating BOW system when there is
        // only 3 topics with life cycle: dilma, futebol and transito
        if(_ignoreRetweetsWithoutTimedTopics && !RetweetHasAnyTimedTopic(_topicLifeSpanMaps, retweet))
            continue;

        auto start = retweet.getCreationTime().addSecs(-_candidatePeriodInHours*3600);
        auto candidates = _user.getCandidates(start, retweet.getCreationTime(), _profileType);
        
        metricsVectors.at(0).push_back(getMetrics(_user.sortCandidates(candidates, retweet.getCreationTime()), retweet));

        for(std::size_t i = 0; i < _topicLifeSpanMaps.size(); i++)
        {
            if(_usePersonalisedLifeSpanMaps && (userTopicLifeSpanMaps.find(i) != userTopicLifeSpanMaps.end()))
            {
                auto mapa = userTopicLifeSpanMaps[i];
                metricsVectors.at(i+1).push_back(getMetrics(_user.sortCandidates(candidates, retweet.getCreationTime(), mapa), retweet));
            }
            else
                metricsVectors.at(i+1).push_back(getMetrics(_user.sortCandidates(candidates, retweet.getCreationTime(), _topicLifeSpanMaps.at(i)), retweet));            
        }
    }
    return metricsVectors;
}

void Evaluation::evaluateSystem(const TwitterUserVector& _users,
                                ProfileType _profileType,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets, 
                                const QDateTime& _endRetweets, 
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps,
                                bool _ignoreRetweetsWithoutTimedTopics,
                                bool _usePersonalisedLifeSpanMaps)
{
    std::ofstream file(_outFileName, std::ios::app);
    for(auto user : _users)
    {
        try {
            auto metricsList = evaluateUser(
                user, 
                _profileType, 
                _startProfile, 
                _endProfile, 
                _startRetweets, 
                _endRetweets, 
                _candidatePeriodInHours, 
                _topicLifeSpanMaps, 
                _ignoreRetweetsWithoutTimedTopics,
                _usePersonalisedLifeSpanMaps
            );
            
            for(std::size_t i = 0; i < metricsList.at(0).size(); i++)
            {
                file << user.getUserId();
                for(auto metrics : metricsList)
                    file << "," << metrics.at(i).MRR() << "," << metrics.at(i).SAt5() << "," << metrics.at(i).SAt10();
                file << std::endl;
            }
        }
        catch(std::exception& _e)
        {
            std::cout << "User: " << user.getUserId() << " - " << _e.what() << std::endl;
        }
    }
}

void Evaluation::evaluateSystem(const std::string& _usersFile,
                                ProfileType _profileType,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets, 
                                const QDateTime& _endRetweets, 
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps,
                                bool _ignoreRetweetsWithoutTimedTopics,
                                bool _usePersonalisedLifeSpanMaps)
{
    TwitterUserVector users;
    std::ifstream file(_usersFile);
    std::string line;
    
    while(std::getline(file, line))
    {
        long userId = atol(line.c_str());
        users.push_back(TwitterUser(userId));
    }
    
    evaluateSystem(
        users, 
        _profileType, 
        _startProfile, 
        _endProfile, 
        _startRetweets, 
        _endRetweets, 
        _candidatePeriodInHours, 
        _outFileName, 
        _topicLifeSpanMaps, 
        _ignoreRetweetsWithoutTimedTopics,
        _usePersonalisedLifeSpanMaps
    );
}

}
