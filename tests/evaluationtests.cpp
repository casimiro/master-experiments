#include <gmock/gmock.h> 
#include "twitteruser.h"
#include <QSqlDatabase>
#include "common.h"
#include "metrics.h"
#include "evaluation.h"

using namespace casimiro;
using namespace ::testing;

class EvaluationTests : public Test {
protected:
    EvaluationTests():user(USER_ID)
    {
    }
    
    virtual ~EvaluationTests()
    {
    }
    
    virtual void SetUp()
    {
        user.loadProfile(START_PROFILE, END_PROFILE);
        auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
        sortedCandidates = user.sortCandidates(candidates);
    }
    
    virtual void TearDown()
    {
        std::remove(RESULT_SYSTEM_FILE_NAME.c_str());
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
        PersistRetweetsData();
    }
    
    static void PersistProfileData()
    {
        QSqlQuery query;
        query.exec("INSERT INTO tweet_topics VALUES (2,'2013-01-01 00:00:00',2256,null,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (3,'2013-01-01 00:05:00',2256,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (4,'2013-01-01 00:10:00',2256,null,'0:0.1 1:0.5','bla brasil')");
    }
    
    static void PersistCandidatesData()
    {
        QSqlQuery query;
        query.exec("INSERT INTO relationship VALUES (2256, 2200)");
        query.exec("INSERT INTO relationship VALUES (2256, 2201)");
        
        query.exec("INSERT INTO tweet_topics VALUES (6,'2013-01-02 00:00:00',2200,null,'0:0.1 3:0.2','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (7,'2013-01-02 00:05:00',2201,null,'0:0.1 2:0.4','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (8,'2013-01-02 00:06:00',2200,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (9,'2013-01-02 00:08:00',2201,null,'0:0.1 1:0.1','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (10,'2013-01-02 00:09:00',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (11,'2013-01-02 00:09:30',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (12,'2013-01-02 00:09:31',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (13,'2013-01-02 00:09:32',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (14,'2013-01-02 00:09:33',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (15,'2013-01-02 00:09:34',2200,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (16,'2013-01-02 00:09:35',2200,null,'0:0.1 1:0.5','bla brasil')");
    }
    
    static void PersistRetweetsData()
    {
        QSqlQuery query;
        query.exec("INSERT INTO tweet_topics VALUES (17,'2013-01-03 00:00:00',2256, 6,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (18,'2013-01-03 00:01:00',2256, 7,'0:0.1 2:0.5','bla usp')");
    }

    std::string RESULT_SYSTEM_FILE_NAME = "result.csv";
    long USER_ID = 2256;
    int HOURS = 48;
    
    QDateTime START_PROFILE = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_PROFILE = QDateTime::fromString("2013-01-01 00:10:01", "yyyy-MM-dd HH:mm:ss");
    
    QDateTime START_CANDIDATES = QDateTime::fromString("2013-01-02 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_CANDIDATES = QDateTime::fromString("2013-01-02 00:10:01", "yyyy-MM-dd HH:mm:ss");
    
    QDateTime START_RETWEETS = QDateTime::fromString("2013-01-03 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_RETWEETS = QDateTime::fromString("2013-01-03 00:10:01", "yyyy-MM-dd HH:mm:ss");
    
    TwitterUser user;
    Evaluation evaluation;
    TweetVector sortedCandidates;
};

TEST_F(EvaluationTests, GetMetricsComputesMRRCorrectly)
{
    auto retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(1).getProfile(), sortedCandidates.at(1).getTweetId());    
    auto metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(0.5, metrics.MRR());
    
    retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(2).getProfile(), sortedCandidates.at(2).getTweetId());    
    metrics = evaluation.getMetrics(sortedCandidates, retweet);
    ASSERT_NEAR(0.333, metrics.MRR(), 0.001);
}

TEST_F(EvaluationTests, GetMetricsComputesSAt5Correctly)
{
    auto retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(1).getProfile(), sortedCandidates.at(1).getTweetId());    
    auto metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(1, metrics.SAt5());
    
    retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(5).getProfile(), sortedCandidates.at(5).getTweetId());    
    metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(0, metrics.SAt5());
}

TEST_F(EvaluationTests, GetMetricsComputesSAt10Correctly)
{
    auto retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(1).getProfile(), sortedCandidates.at(1).getTweetId());    
    auto metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(1, metrics.SAt10());
    
    retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(10).getProfile(), sortedCandidates.at(10).getTweetId());    
    metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(0, metrics.SAt10());
}

TEST_F(EvaluationTests, GetMetricsReturnsZeroValuesWhenRetweetIsNotInTheSortedList)
{
    auto retweet = Tweet(20, END_CANDIDATES, sortedCandidates.at(1).getProfile(), 999);    
    auto metrics = evaluation.getMetrics(sortedCandidates, retweet);
    
    ASSERT_EQ(0, metrics.MRR());
    ASSERT_EQ(0, metrics.SAt5());
    ASSERT_EQ(0, metrics.SAt10());
}

TEST_F(EvaluationTests, EvaluateUserGeneratesAListOfCorrectMetrics)
{
    auto metrics = evaluation.evaluateUser(user, START_PROFILE, END_PROFILE, START_RETWEETS, END_RETWEETS, HOURS);
    
    ASSERT_EQ(2, metrics.size()); // There are 2 retweets in the db
    
    ASSERT_EQ(1, metrics.at(0).MRR());
    ASSERT_EQ(1, metrics.at(0).SAt5());
    ASSERT_EQ(1, metrics.at(0).SAt10());
    
    ASSERT_NEAR(0.333, metrics.at(1).MRR(), 0.01);
    ASSERT_EQ(1, metrics.at(1).SAt5());
    ASSERT_EQ(1, metrics.at(1).SAt10());
}

TEST_F(EvaluationTests, EvaluateSystemGeneratesAFileWithTheMetricsList)
{
    TwitterUserVector users{user};
    evaluation.evaluateSystem(users, START_PROFILE, END_PROFILE, START_RETWEETS, END_RETWEETS, HOURS, RESULT_SYSTEM_FILE_NAME);
    
    std::ifstream file(RESULT_SYSTEM_FILE_NAME);
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    std::ifstream expectedFile("expected_result.csv");
    std::string expectedFileContent((std::istreambuf_iterator<char>(expectedFile)), std::istreambuf_iterator<char>());
    
    ASSERT_EQ(expectedFileContent, fileContent);
}

TEST_F(EvaluationTests, EvaluateSystemUsingUsersFileGeneratesAFileWithTheMetricsList)
{
    evaluation.evaluateSystem("users.csv", START_PROFILE, END_PROFILE, START_RETWEETS, END_RETWEETS, HOURS, RESULT_SYSTEM_FILE_NAME);
    
    std::ifstream file(RESULT_SYSTEM_FILE_NAME);
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    std::ifstream expectedFile("expected_result.csv");
    std::string expectedFileContent((std::istreambuf_iterator<char>(expectedFile)), std::istreambuf_iterator<char>());
    
    ASSERT_EQ(expectedFileContent, fileContent);
}