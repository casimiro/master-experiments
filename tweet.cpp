#include "tweet.h"

namespace casimiro {

Tweet::Tweet(long int _tweetId, QDateTime _creationTime, StringFloatMap _profile, long _retweetedId):
    m_tweetId(_tweetId),
    m_creationTime(_creationTime),
    m_profile(_profile),
    m_retweetedId(_retweetedId)
{
}

Tweet::~Tweet()
{
}

long int Tweet::getTweetId() const
{
    return m_tweetId;
}

const StringFloatMap& Tweet::getProfile() const
{
    return m_profile;
}

const QDateTime& Tweet::getCreationTime() const
{
    return m_creationTime;
}

long int Tweet::getRetweetedId() const
{
    return m_retweetedId;
}

}