#ifndef TWEET_H
#define TWEET_H

#include <vector>

namespace casimiro {

class Tweet;
typedef std::vector<Tweet> TweetVector;
    
class Tweet
{
public:
    Tweet();
    virtual ~Tweet();
};

}
#endif // TWEET_H
