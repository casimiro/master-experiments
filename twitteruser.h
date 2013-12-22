#include <map>
#include <string>
#include <QDateTime>
#include "tweet.h"

#ifndef TWITTERUSER_H
#define TWITTERUSER_H

namespace casimiro {

typedef std::map<std::string, float> StringFloatMap;    

class TwitterUser {
public:
    TwitterUser(long _userId);
    ~TwitterUser() {}
    
    virtual long getUserId() const;
    
    virtual void loadProfile(const QDateTime& _start, const QDateTime& _end);
    
    virtual const StringFloatMap& getProfile() const;
    
    virtual TweetVector getCandidates(const QDateTime& _start, const QDateTime& _end) const;
    
private:
    long m_userId;
    StringFloatMap m_profile;
};

}
#endif // TWITTERUSER_H
