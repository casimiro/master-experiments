#include "tweet.h"

namespace casimiro {

Tweet::Tweet(long int _tweetId):
    m_tweetId(_tweetId)
{
}

Tweet::~Tweet()
{

}

long int Tweet::getTweetId() const
{
    return m_tweetId;
}
    
}