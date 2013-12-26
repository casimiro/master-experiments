#ifndef EVALUATION_H
#define EVALUATION_H

#include "twitteruser.h"
#include "metrics.h"
#include "tweet.h"

namespace casimiro {

class Evaluation {
public:
    Evaluation();
    virtual ~Evaluation();
    
    virtual Metrics getMetrics(const TweetVector& _sortedCandidates, const Tweet& _retweet) const;
    
    virtual MetricsVector evaluateUser(TwitterUser& _user, 
                                       const QDateTime& _startProfile, 
                                       const QDateTime& _endProfile, 
                                       const QDateTime& _startRetweets,
                                       const QDateTime& _endRetweets,
                                       int _candidatePeriodInHours);
    
    virtual void evaluateSystem(const TwitterUserVector& _users, 
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets,
                                const QDateTime& _endRetweets,
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName);
};

}
#endif // EVALUATION_H
