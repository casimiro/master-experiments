#ifndef TWEET_H
#define TWEET_H

#include <vector>
#include "utils.h"

namespace casimiro {

class Tweet;
typedef std::vector<Tweet> TweetVector;
    
class Tweet
{
public:
    Tweet(long _tweetId, StringFloatMap _profile);
    virtual ~Tweet();
    
    virtual long getTweetId() const;
    
    virtual const StringFloatMap& getProfile() const;
    
private:
    long m_tweetId;
    StringFloatMap m_profile;
};

}
#endif // TWEET_H
