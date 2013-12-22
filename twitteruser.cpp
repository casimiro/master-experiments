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
        candidates.push_back(Tweet(query.value(0).toLongLong()));
    
    return candidates;
}


}