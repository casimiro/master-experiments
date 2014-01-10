#include "twitteruser.h"

#include <boost/algorithm/string.hpp>
#include <QSql>
#include <QSqlQuery>
#include <QVariant>
#include "profilenotloadederror.h"

namespace casimiro {

TwitterUser::TwitterUser(long int _userId):
    m_userId(_userId)
{
}

TwitterUser::~TwitterUser()
{

}

long int TwitterUser::getUserId() const
{
    return m_userId;
}

void TwitterUser::loadProfile(const QDateTime& _start, const QDateTime& _end)
{
    QSqlQuery query;
    query.prepare("SELECT topics FROM tweet_topics WHERE user_id=:uid AND creation_time >= :s AND creation_time <= :e"); 
    query.bindValue(":uid", static_cast<qlonglong>(m_userId));
    query.bindValue(":s", _start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":e", _end.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
    
    float val = 0;
    
    std::vector<std::string> pairs;
    size_t colonPos;
    std::string topic;
    
    while(query.next())
    {
        pairs.clear();
        std::string content = query.value(0).toString().toStdString();
        boost::split(pairs, content, boost::is_any_of(" "));
        for (auto pair : pairs)
        {
            colonPos = pair.find(":");
            topic = pair.substr(0, colonPos).c_str();
            val = atof(pair.substr(colonPos+1, pair.size() - colonPos).c_str());
            m_profile[topic] += val;
        }
    }
    
}

void TwitterUser::loadBOWProfile(const QDateTime& _start, const QDateTime& _end)
{
    QSqlQuery query;
    query.prepare("SELECT content FROM tweet_topics WHERE user_id=:uid AND creation_time >= :s AND creation_time <= :e"); 
    query.bindValue(":uid", static_cast<qlonglong>(m_userId));
    query.bindValue(":s", _start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":e", _end.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
    
    int totalTokens = 0;
    
    while(query.next())
    {
        std::vector<std::string> tokens;
        auto content = query.value(0).toString().toStdString();
        boost::split(tokens, content, boost::is_any_of(" "));
        for (auto token : tokens)
        {
            m_profile[token] += 1;
            totalTokens++;
        }
    }
    
    for(auto it = m_profile.begin(); it != m_profile.end(); it++)
        it->second /= totalTokens;
}

const StringFloatMap& TwitterUser::getProfile() const
{
    return m_profile;
}

TweetVector TwitterUser::getCandidates(const QDateTime& _start, const QDateTime& _end, ProfileType _profileType) const
{
    TweetVector candidates;
    QSqlQuery query;
    query.prepare(
        "SELECT id, topics, creation_time, content FROM tweet_topics WHERE user_id in (SELECT followed_id FROM relationship WHERE follower_id = :uid) "
        "AND creation_time BETWEEN :s AND :e"
    );
    query.bindValue(":uid", static_cast<qlonglong>(m_userId));
    query.bindValue(":s", _start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":e", _end.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
    
    while(query.next())
    {
        switch(_profileType)
        {
            case TopicProfile:
                candidates.push_back(Tweet(query.value(0).toLongLong(), query.value(2).toDateTime(), BuildProfileFromString(query.value(1).toString())));
                break;
            case BOWProfile:
                candidates.push_back(Tweet(query.value(0).toLongLong(), query.value(2).toDateTime(), BuildBOWProfileFromString(query.value(3).toString())));
                break;
        }
    }
    return candidates;
}

TweetVector TwitterUser::getRetweets(const QDateTime& _start, const QDateTime& _end) const
{
    TweetVector retweets;
    QSqlQuery query;
    query.prepare("SELECT id, topics, creation_time, retweeted FROM tweet_topics WHERE retweeted IS NOT NULL AND user_id = :uid AND creation_time BETWEEN :s AND :e");
    query.bindValue(":uid", static_cast<qlonglong>(m_userId));
    query.bindValue(":s", _start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":e", _end.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
    
    while(query.next())
    {
        StringFloatMap profile = BuildProfileFromString(query.value(1).toString());
        retweets.push_back(Tweet(query.value(0).toLongLong(), query.value(2).toDateTime(), profile, query.value(3).toLongLong()));
    }
    return retweets;
}

float TwitterUser::cosineSimilarity(const StringFloatMap& _profile) const
{
    float aNorm,bNorm,dot;
    aNorm = bNorm = dot = 0;
    
    auto uIt = m_profile.begin();
    auto nIt = _profile.begin();
    
    auto uEnd = m_profile.end();
    auto nEnd = _profile.end();
    
    int compare;
    while (uIt != uEnd && nIt != nEnd)
    {
        compare = uIt->first.compare(nIt->first);
        if(compare == 0)
        {
            dot += uIt->second * nIt->second;
            uIt++;
            nIt++;
        }
        else if(compare < 0)
            uIt++;
        else if(compare > 0)
            nIt++;
    }
    
    for (uIt = m_profile.begin(); uIt != uEnd; uIt++)
        aNorm += pow(uIt->second, 2);
    aNorm = sqrt(aNorm);
    
    for (nIt = _profile.begin(); nIt != nEnd; nIt++)
        bNorm += pow(nIt->second, 2);
    bNorm = sqrt(bNorm);
    
    return dot / (aNorm * bNorm);
}

TweetVector TwitterUser::sortCandidates(const TweetVector& _candidates, const QDateTime& _recommendationTime, const StringIntMap& _topicLifeSpan) const
{
    if(m_profile.empty())
        throw ProfileNotLoadedError();
    
    TweetVector sorted;
    std::map<float, std::vector<int>> aux;
    int i = 0;
    
    for(auto candidate : _candidates)
    {
        if(!CandidateHasOldTopics(candidate, _topicLifeSpan, _recommendationTime))
        {
            auto sim = cosineSimilarity(candidate.getProfile());
            if(aux.find(sim) == aux.end())
                aux[sim] = std::vector<int>();
            aux.find(sim)->second.push_back(i);
        }
        i++;
    }
    
    for(auto it = aux.rbegin(); it != aux.crend(); it++)
        for(auto index : it->second)
            sorted.push_back(_candidates.at(index));
    
    return sorted;
}

bool TwitterUser::CandidateHasOldTopics(const Tweet& _candidate, const StringIntMap& _topicLifeSpan, const QDateTime& _retweetCreationTime)
{
    int lifeInSeconds = (_retweetCreationTime.toMSecsSinceEpoch() - _candidate.getCreationTime().toMSecsSinceEpoch()) / 1000;
    for(auto pair : _candidate.getProfile())
    {
        auto it = _topicLifeSpan.find(pair.first);
        if(it != _topicLifeSpan.end() && lifeInSeconds > it->second)
            return true;
    }
    return false;
}

}