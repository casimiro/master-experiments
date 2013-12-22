#ifndef TWEET_H
#define TWEET_H

#include <vector>
#include <QDateTime>
#include "utils.h"

namespace casimiro {

class Tweet;
typedef std::vector<Tweet> TweetVector;
    
class Tweet
{
public:
    Tweet(long _tweetId, QDateTime _creationTime, StringFloatMap _profile);
    virtual ~Tweet();
    
    virtual long getTweetId() const;
    
    virtual const StringFloatMap& getProfile() const;
    
    virtual const QDateTime& getCreationTime() const;
    
private:
    long m_tweetId;
    QDateTime m_creationTime;
    StringFloatMap m_profile;
};

}
#endif // TWEET_H
