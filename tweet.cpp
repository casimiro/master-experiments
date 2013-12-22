#include "tweet.h"

namespace casimiro {

Tweet::Tweet(long int _tweetId, casimiro::StringFloatMap _profile):
    m_tweetId(_tweetId),
    m_profile(_profile)
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
   
}