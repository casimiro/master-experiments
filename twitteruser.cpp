#include "twitteruser.h"

#include <boost/algorithm/string.hpp>
#include <QSql>
#include <QSqlQuery>
#include <QVariant>

namespace casimiro {

TwitterUser::TwitterUser(long int _userId):
    m_userId(_userId)
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

const StringFloatMap& TwitterUser::getProfile() const
{
    return m_profile;
}

TweetVector TwitterUser::getCandidates(const QDateTime& _start, const QDateTime& _end) const
{
    TweetVector candidates;
    QSqlQuery query;
    query.prepare(
        "SELECT id, topics, creation_time FROM tweet_topics WHERE user_id in (SELECT followed_id FROM relationship WHERE follower_id = :uid) "
        "AND creation_time BETWEEN :s AND :e"
    );
    query.bindValue(":uid", static_cast<qlonglong>(m_userId));
    query.bindValue(":s", _start.toString("yyyy-MM-dd HH:mm:ss"));
    query.bindValue(":e", _end.toString("yyyy-MM-dd HH:mm:ss"));
    query.exec();
    
    while(query.next())
    {
        StringFloatMap profile = BuildProfileFromString(query.value(1).toString());
        candidates.push_back(Tweet(query.value(0).toLongLong(), query.value(2).toDateTime(), profile));
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

}