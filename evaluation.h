#ifndef EVALUATION_H
#define EVALUATION_H

#include "metrics.h"
#include "tweet.h"

namespace casimiro {

class Evaluation {
public:
    Evaluation();
    virtual ~Evaluation();
    
    virtual Metrics getMetrics(const TweetVector &_sortedCandidates, const Tweet &_retweet) const;
};

}
#endif // EVALUATION_H
