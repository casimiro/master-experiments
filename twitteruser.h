#include <string>
#include <QDateTime>
#include "utils.h"
#include "tweet.h"
#include <vector>
#include "svm.h"

#ifndef TWITTERUSER_H
#define TWITTERUSER_H

namespace casimiro {

using std::vector;

enum ProfileType {
    TopicProfile,
    BOWProfile
};

class TwitterUser;
typedef vector<TwitterUser> TwitterUserVector;
typedef struct svm_model SVMModel;

class TwitterUser {
public:
    TwitterUser(long _userId);
    virtual ~TwitterUser();
    
    virtual long getUserId() const;
    
    virtual void loadSVMProfile();
    
    virtual void loadProfile(const QDateTime& _start, const QDateTime& _end);
    
    virtual void loadBOWProfile(const QDateTime& _start, const QDateTime& _end);
    
    virtual const StringFloatMap& getProfile() const;
    
    virtual TweetVector getCandidates(const QDateTime& _start, const QDateTime& _end, ProfileType _profileType) const;
    
    virtual TweetVector getRetweets(const QDateTime& _start, const QDateTime& _end) const;
    
    virtual float cosineSimilarity(const StringFloatMap& _profile) const;
    
    virtual TweetVector svmSortCandidates(const TweetVector& _candidates, const QDateTime& _recommendationTime, const StringIntMap& _topicLifeSpan = StringIntMap()) const;
    
    virtual TweetVector sortCandidates(const TweetVector& _candidates, const QDateTime& _recommendationTime, const StringIntMap& _topicLifeSpan = StringIntMap()) const;
    
private:
    long m_userId;
    StringFloatMap m_profile;
    SVMModel* m_svmModel = nullptr;
    
    static bool CandidateHasOldTopics(const Tweet& _candidate, const StringIntMap& _topicLifeSpan, const QDateTime& _retweetCreationTime);
};

}
#endif // TWITTERUSER_H
