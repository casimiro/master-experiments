#include "evaluation.h"
#include <fstream>
#include <iostream>

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

MetricsVector Evaluation::evaluateUser(TwitterUser& _user, 
                                       const QDateTime& _startProfile, 
                                       const QDateTime& _endProfile, 
                                       const QDateTime& _startRetweets, 
                                       const QDateTime& _endRetweets, 
                                       int _candidatePeriodInHours,
                                       const StringIntMap& _topicLifeSpanMap)
{
    MetricsVector metrics;
    
    _user.loadProfile(_startProfile, _endProfile);
    auto retweets = _user.getRetweets(_startRetweets, _endRetweets);
    for(auto retweet : retweets)
    {
        auto start = retweet.getCreationTime().addSecs(-_candidatePeriodInHours*3600);
        auto candidates = _user.getCandidates(start, retweet.getCreationTime(), TopicProfile);
        metrics.push_back(getMetrics(_user.sortCandidates(candidates, retweet.getCreationTime(), _topicLifeSpanMap), retweet));
    }
    return metrics;
}

void Evaluation::evaluateSystem(const TwitterUserVector& _users,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets, 
                                const QDateTime& _endRetweets, 
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps)
{
    std::ofstream file(_outFileName);
    for(auto user : _users)
    {
        try {
            auto metricsList = std::vector<MetricsVector>();
            metricsList.push_back(evaluateUser(user, _startProfile, _endProfile, _startRetweets, _endRetweets, _candidatePeriodInHours));
            
            for(auto _topicLifeSpanMap : _topicLifeSpanMaps)
                metricsList.push_back(evaluateUser(user, _startProfile, _endProfile, _startRetweets, _endRetweets, _candidatePeriodInHours, _topicLifeSpanMap));
            
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
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets, 
                                const QDateTime& _endRetweets, 
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps)
{
    TwitterUserVector users;
    std::ifstream file(_usersFile);
    std::string line;
    
    while(std::getline(file, line))
    {
        long userId = atol(line.c_str());
        users.push_back(TwitterUser(userId));
    }
    
    evaluateSystem(users, _startProfile, _endProfile, _startRetweets, _endRetweets, _candidatePeriodInHours, _outFileName, _topicLifeSpanMaps);
}

}