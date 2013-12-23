#include <string>
#include <QDateTime>
#include "utils.h"
#include "tweet.h"

#ifndef TWITTERUSER_H
#define TWITTERUSER_H

namespace casimiro {

class TwitterUser {
public:
    TwitterUser(long _userId);
    ~TwitterUser() {}
    
    virtual long getUserId() const;
    
    virtual void loadProfile(const QDateTime& _start, const QDateTime& _end);
    
    virtual const StringFloatMap& getProfile() const;
    
    virtual TweetVector getCandidates(const QDateTime& _start, const QDateTime& _end) const;
    
    virtual TweetVector getRetweets(const QDateTime& _start, const QDateTime& _end) const;
    
    virtual float cosineSimilarity(const StringFloatMap& _profile) const;
    
    virtual TweetVector sortCandidates(const TweetVector& _candidates) const;
    
private:
    long m_userId;
    StringFloatMap m_profile;
};

}
#endif // TWITTERUSER_H
