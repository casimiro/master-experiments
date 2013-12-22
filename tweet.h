#ifndef TWEET_H
#define TWEET_H

#include <vector>

namespace casimiro {

class Tweet;
typedef std::vector<Tweet> TweetVector;
    
class Tweet
{
public:
    Tweet(long _tweetId);
    virtual ~Tweet();
    
    virtual long getTweetId() const;
    
private:
    long m_tweetId;
};

}
#endif // TWEET_H
