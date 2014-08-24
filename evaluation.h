#ifndef EVALUATION_H
#define EVALUATION_H

#include "twitteruser.h"
#include "metrics.h"
#include "tweet.h"

namespace casimiro {

typedef std::vector<MetricsVector> MetricsVectors;

class Evaluation {
public:
    Evaluation();
    virtual ~Evaluation();
    
    virtual Metrics getMetrics(const TweetVector& _sortedCandidates, const Tweet& _retweet) const;
    
    virtual MetricsVectors evaluateUser(TwitterUser& _user, 
                                ProfileType _profileType,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets,
                                const QDateTime& _endRetweets,
                                int _candidatePeriodInHours,
                                const StringIntMaps& _topicLifeSpanMaps = StringIntMaps(),
                                bool _ignoreRetweetsWithoutTimedTopics = false,
                                bool _usePersonalisedLifeSpanMaps = false);
    
    virtual void evaluateSystem(const TwitterUserVector& _users,
                                ProfileType _profileType,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets,
                                const QDateTime& _endRetweets,
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps = StringIntMaps(),
                                bool _ignoreRetweetsWithoutTimedTopics = false,
                                bool _usePersonalisedLifeSpanMaps = false);
    
    virtual void evaluateSystem(const std::string& _usersFile, 
                                ProfileType _profileType,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets,
                                const QDateTime& _endRetweets,
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName,
                                const StringIntMaps& _topicLifeSpanMaps = StringIntMaps(),
                                bool _ignoreRetweetsWithoutTimedTopics = false,
                                bool _usePersonalisedLifeSpanMaps = false);
};

}
#endif // EVALUATION_H
