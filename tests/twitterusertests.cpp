#include <iostream>
#include <gmock/gmock.h> 
#include "twitteruser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <fstream>
#include "profilenotloadederror.h"
#include "common.h"

using namespace ::testing;
using namespace casimiro;


class TwitterUserTest: public Test {
protected:
    TwitterUserTest():user(USER_ID)
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
        PersistRetweetsData();
    }
    
    virtual void SetUp()
    {
        user = TwitterUser(USER_ID);
    }
    
    virtual void TearDown()
    {
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
    
    static void PersistRetweetsData()
    {
        QSqlQuery query;
        
        // Useful data
        query.exec("INSERT INTO tweet_topics VALUES (12,'2013-01-03 00:00:00',2256, 6,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (13,'2013-01-03 00:05:00',2256, 7,'0:0.1 2:0.5','bla usp')");
        
        // Noise data
        query.exec("INSERT INTO tweet_topics VALUES (14,'2013-01-03 00:08:00',2200, 6,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (13,'2013-01-03 00:15:00',2256, 9,'0:0.1 1:0.5','bla brasil')");
    }
    
    long USER_ID = 2256;
    QDateTime START_PROFILE = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_PROFILE = QDateTime::fromString("2013-01-01 00:10:01", "yyyy-MM-dd HH:mm:ss");

    QDateTime START_CANDIDATES = QDateTime::fromString("2013-01-02 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_CANDIDATES = QDateTime::fromString("2013-01-02 00:10:01", "yyyy-MM-dd HH:mm:ss");

    QDateTime START_RETWEETS = QDateTime::fromString("2013-01-03 00:00:00", "yyyy-MM-dd HH:mm:ss");
    QDateTime END_RETWEETS = QDateTime::fromString("2013-01-03 00:10:01", "yyyy-MM-dd HH:mm:ss");
    
    TwitterUser user;
};


TEST_F(TwitterUserTest, TwitterUserIsCreatedWithTheGivenId)
{
    ASSERT_EQ(user.getUserId(), USER_ID);
}

TEST_F(TwitterUserTest, ProfileLoading)
{
    user.loadProfile(START_PROFILE, END_PROFILE);
    auto profile = user.getProfile();
    
    ASSERT_NEAR(profile.find("0")->second, 0.3, 0.01);
    ASSERT_NEAR(profile.find("1")->second, 0.5, 0.01);
    ASSERT_NEAR(profile.find("2")->second, 0.5, 0.01);
    ASSERT_NEAR(profile.find("3")->second, 0.5, 0.01);
}

TEST_F(TwitterUserTest, GetCandidatesTweetsOnlyReturnsTweetsPublishedByFollowedUsersCreatedInTheGivenInterval)
{
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    
    ASSERT_EQ(candidates.size(), 4);
    ASSERT_EQ(candidates.at(0).getTweetId(), 6);
    ASSERT_EQ(candidates.at(1).getTweetId(), 7);
    ASSERT_EQ(candidates.at(2).getTweetId(), 9);
    ASSERT_EQ(candidates.at(3).getTweetId(), 10);
};

TEST_F(TwitterUserTest, GetCandidatesReturnsTweetsWithCorrectCreationTime)
{
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    
    ASSERT_EQ(candidates.at(0).getCreationTime(), QDateTime::fromString("2013-01-02 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    ASSERT_EQ(candidates.at(1).getCreationTime(), QDateTime::fromString("2013-01-02 00:05:00", "yyyy-MM-dd HH:mm:ss"));
    ASSERT_EQ(candidates.at(2).getCreationTime(), QDateTime::fromString("2013-01-02 00:08:00", "yyyy-MM-dd HH:mm:ss"));
    ASSERT_EQ(candidates.at(3).getCreationTime(), QDateTime::fromString("2013-01-02 00:10:00", "yyyy-MM-dd HH:mm:ss"));
    
}

TEST_F(TwitterUserTest, GetCandidatesLoadsTweetProfilesCorrectely)
{
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    
    auto profile = candidates.at(0).getProfile();
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.01);
    ASSERT_NEAR(profile.find("3")->second, 0.2, 0.01);
    
    profile = candidates.at(1).getProfile();
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.01);
    ASSERT_NEAR(profile.find("2")->second, 0.4, 0.01);
    
    profile = candidates.at(2).getProfile();
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.01);
    ASSERT_NEAR(profile.find("1")->second, 0.1, 0.01);
}

TEST_F(TwitterUserTest, GetRetweetsReturnsOnlyRetweetsPublishedByTheUserInTheGivenPeriod)
{
    auto retweets = user.getRetweets(START_RETWEETS, END_RETWEETS);
    
    ASSERT_EQ(retweets.size(), 2);
    ASSERT_EQ(retweets.at(0).getTweetId(), 12);
    ASSERT_EQ(retweets.at(1).getTweetId(), 13);
}

TEST_F(TwitterUserTest, GetRetweetsReturnsRetweetsWithCorrectRetweetedId)
{
    auto retweets = user.getRetweets(START_RETWEETS, END_RETWEETS);

    ASSERT_EQ(retweets.at(0).getRetweetedId(), 6);
    ASSERT_EQ(retweets.at(1).getRetweetedId(), 7);
}

TEST_F(TwitterUserTest, GetRetweetsReturnsRetweetsWithCorrectCreationTime)
{
    auto retweets = user.getRetweets(START_RETWEETS, END_RETWEETS);
    
    ASSERT_EQ(retweets.at(0).getCreationTime(), QDateTime::fromString("2013-01-03 00:00:00", "yyyy-MM-dd HH:mm:ss"));
    ASSERT_EQ(retweets.at(1).getCreationTime(), QDateTime::fromString("2013-01-03 00:05:00", "yyyy-MM-dd HH:mm:ss"));
}

TEST_F(TwitterUserTest, GetRetweetsReturnsRetweetsWithCorrectProfile)
{
    auto retweets = user.getRetweets(START_RETWEETS, END_RETWEETS);
    
    auto profile = retweets.at(0).getProfile();
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.01);
    ASSERT_NEAR(profile.find("3")->second, 0.5, 0.01);
    
    profile = retweets.at(1).getProfile();
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.01);
    ASSERT_NEAR(profile.find("2")->second, 0.5, 0.01);
}

TEST_F(TwitterUserTest, ComputesCosineSimilarityCorrectly)
{
    StringFloatMap profile;
    user.loadProfile(START_PROFILE, END_PROFILE);
    
    profile["0"] = 0.1;
    profile["3"] = 0.2;
    ASSERT_NEAR(0.634335, user.cosineSimilarity(profile), 0.000001);
    
    profile.clear();
    profile["0"] = 0.1;
    profile["2"] = 0.4;
    ASSERT_NEAR(0.6086445, user.cosineSimilarity(profile), 0.000001);
    
    profile.clear();
    profile["0"] = 0.1;
    profile["1"] = 0.1;
    ASSERT_NEAR(0.617213, user.cosineSimilarity(profile), 0.000001);
}

TEST_F(TwitterUserTest, SortCandidatesRaisesExceptionWhenProfileNotLoaded)
{
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    
    ASSERT_THROW(user.sortCandidates(candidates, QDateTime()), ProfileNotLoadedError);
}

TEST_F(TwitterUserTest, SortCandidatesWithRespectTheCosineSimilarityFunction)
{
    user.loadProfile(START_PROFILE, END_PROFILE);
    
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    auto sorted = user.sortCandidates(candidates, QDateTime());
    
    ASSERT_EQ(6, sorted.at(0).getTweetId());
    ASSERT_EQ(9, sorted.at(1).getTweetId());
    ASSERT_EQ(7, sorted.at(2).getTweetId());
    ASSERT_EQ(10, sorted.at(3).getTweetId());
}

TEST_F(TwitterUserTest, SortCandidatesApplyingSomeTopicFilter)
{
    user.loadProfile(START_PROFILE, END_PROFILE);
    
    std::map<std::string, int> topicFilter;
    topicFilter["3"] = 3600*12;
    auto candidates = user.getCandidates(START_CANDIDATES, END_CANDIDATES);
    auto sorted = user.sortCandidates(candidates, START_RETWEETS, topicFilter);
    
    ASSERT_EQ(3, sorted.size());
    ASSERT_EQ(9, sorted.at(0).getTweetId());
    ASSERT_EQ(7, sorted.at(1).getTweetId());
    ASSERT_EQ(10, sorted.at(2).getTweetId());
}