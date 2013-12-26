#include "evaluation.h"
#include <fstream>

namespace casimiro {

Evaluation::Evaluation()
{
}

Evaluation::~Evaluation()
{
}

Metrics Evaluation::getMetrics(const TweetVector& _sortedCandidates, const Tweet& _retweet) const
{
    int i = 0;
    int pos = -1;
    for(auto tweet : _sortedCandidates)
    {
        if(tweet.getTweetId() == _retweet.getRetweetedId())
        {
            pos = i;
            break;
        }
        i++;
    }
    return Metrics(pos);
}

MetricsVector Evaluation::evaluateUser(TwitterUser& _user, 
                                       const QDateTime& _startProfile, 
                                       const QDateTime& _endProfile, 
                                       const QDateTime& _startRetweets, 
                                       const QDateTime& _endRetweets, 
                                       int _candidatePeriodInHours)
{
    MetricsVector metrics;
    
    _user.loadProfile(_startProfile, _endProfile);
    auto retweets = _user.getRetweets(_startRetweets, _endRetweets);
    for(auto retweet : retweets)
    {
        auto start = retweet.getCreationTime().addSecs(-_candidatePeriodInHours*3600);
        auto candidates = _user.getCandidates(start, retweet.getCreationTime());
        metrics.push_back(getMetrics(_user.sortCandidates(candidates), retweet));
    }
    return metrics;
}

void Evaluation::evaluateSystem(const TwitterUserVector& _users,
                                const QDateTime& _startProfile, 
                                const QDateTime& _endProfile, 
                                const QDateTime& _startRetweets, 
                                const QDateTime& _endRetweets, 
                                int _candidatePeriodInHours, 
                                const std::string& _outFileName)
{
    std::ofstream file(_outFileName);
    for(auto user : _users)
    {
        auto metrics = evaluateUser(user, _startProfile, _endProfile, _startRetweets, _endRetweets, _candidatePeriodInHours);
        for(auto metric : metrics)
            file << user.getUserId() << "," << metric.MRR() << "," << metric.SAt5() << "," << metric.SAt10() << std::endl;
    }
}

}