#include <gmock/gmock.h> 
#include "twitteruser.h"
#include <QSqlDatabase>
#include "common.h"

using namespace casimiro;
using namespace ::testing;


class Metrics {
public:
    Metrics(float _MRR):m_MRR(_MRR) {}
    virtual ~Metrics() {}
    
    virtual float MRR() const 
    {
        return m_MRR;
    }
    
private:
    float m_MRR;
};

class Evaluation {
public:
    Evaluation(){}
    virtual ~Evaluation() {}
    
    virtual Metrics getMetrics(const TweetVector &_sortedCandidates, const Tweet &_retweet)
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
        
        float MRR = 1.0 / (pos+1);
        return Metrics(MRR);
    }
};

class EvaluationTests : public Test {
protected:
    EvaluationTests()
    {
    }
    
    virtual ~EvaluationTests()
    {
    }
    
    static void SetUpTestCase()
    {
        if(!DB.isOpen())
        {
            DB.setDatabaseName("casimiro.db.sqlite");
            DB.open();
        }
        DropTables();
        CreateDbStructure();
        PersistProfileData();
        PersistCandidatesData();
    }
    
    static void PersistProfileData()
    {
        QSqlQuery query;
        // Loading useful data
        query.exec("INSERT INTO tweet_topics VALUES (2,'2013-01-01 00:00:00',2256,null,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (3,'2013-01-01 00:05:00',2256,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (4,'2013-01-01 00:10:00',2256,null,'0:0.1 1:0.5','bla brasil')");
        
        // Loading noise data
        query.exec("INSERT INTO tweet_topics VALUES (1,'2012-12-31 23:55:00',2256,null,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (5,'2013-01-01 00:15:00',2256,null,'0:0.1 2:0.5','bla usp')");
    }
    
    static void PersistCandidatesData()
    {
        QSqlQuery query;
        
        // Useful data
        query.exec("INSERT INTO relationship VALUES (2256, 2200)");
        query.exec("INSERT INTO relationship VALUES (2256, 2201)");
        
        query.exec("INSERT INTO tweet_topics VALUES (6,'2013-01-02 00:00:00',2200,null,'0:0.1 3:0.2','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (7,'2013-01-02 00:05:00',2201,null,'0:0.1 2:0.4','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (9,'2013-01-02 00:08:00',2201,null,'0:0.1 1:0.1','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (10,'2013-01-02 00:10:00',2200,null,'0:0.1 1:0.5','bla brasil')");
        
        // Noise data
        query.exec("INSERT INTO tweet_topics VALUES (8,'2013-01-02 00:05:00',2202,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (11,'2013-01-02 00:15:00',2200,null,'0:0.1 1:0.5','bla brasil')");
    }

    long USER_ID = 2256;
    
    QDateTime START_PROFILE = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_PROFILE = QDateTime::fromString("2013-01-01 00:10:01", "yyyy-MM-dd HH:mm:ss");
    
    QDateTime START_CANDIDATES = QDateTime::fromString("2013-01-02 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_CANDIDATES = QDateTime::fromString("2013-01-02 00:10:01", "yyyy-MM-dd HH:mm:ss");
};

TEST_F(EvaluationTests, GetMetricsComputesMRRCorrectly)
{
    TwitterUser user(USER_ID);
    user.loadProfile(START_PROFILE, END_PROFILE);
    Evaluation evaluation;
    
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    auto sorted = user.sortCandidates(candidates);
    auto retweet = Tweet(20, END_CANDIDATES, sorted.at(1).getProfile(), sorted.at(1).getTweetId());    
    auto metrics = evaluation.getMetrics(sorted, retweet);
    
    ASSERT_EQ(0.5, metrics.MRR());
    
    retweet = Tweet(20, END_CANDIDATES, sorted.at(2).getProfile(), sorted.at(2).getTweetId());    
    metrics = evaluation.getMetrics(sorted, retweet);
    ASSERT_NEAR(0.333, metrics.MRR(), 0.001);
}