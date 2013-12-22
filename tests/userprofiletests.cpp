#include <iostream>
#include <gmock/gmock.h> 
#include "twitteruser.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <fstream>

using namespace ::testing;
using namespace casimiro;

long USER_ID = 2256;
QDateTime startProfile = QDateTime::fromString("2013-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
QDateTime endProfile = QDateTime::fromString("2013-01-01 00:10:01", "yyyy-MM-dd HH:mm:ss");

QDateTime startCandidates = QDateTime::fromString("2013-01-02 00:00:00", "yyyy-MM-dd HH:mm:ss");
QDateTime endCandidates = QDateTime::fromString("2013-01-02 00:10:01", "yyyy-MM-dd HH:mm:ss");

QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

class TwitterUserTest: public Test {
protected:
    TwitterUserTest():user(USER_ID)
    {
        if(!db.isOpen())
        {
            db.setDatabaseName("casimiro.db.sqlite");
            db.open();
        }
    }
    
    virtual void SetUp()
    {
        user = TwitterUser(USER_ID);
        createDbStructure();
        persistProfileData();
        persistCandidatesData();
    }
    
    virtual void TearDown()
    {
        cleanUpDb();
    }
    
    virtual void cleanUpDb()
    {
        std::ifstream file("teardown.sql");
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        QSqlQuery query;
        query.exec(content.c_str());
    }
    
    virtual void createDbStructure()
    {
        QSqlQuery query;
        std::ifstream file("tweet_topics.sql");
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        query.exec(content.c_str());
        file.close();
        
        file.open("relationship.sql");
        content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        query.exec(content.c_str());
    }
    
    virtual void persistProfileData()
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
    
    virtual void persistCandidatesData()
    {
        QSqlQuery query;
        
        // Usefull data
        query.exec("INSERT INTO relationship VALUES (2256, 2200)");
        query.exec("INSERT INTO relationship VALUES (2256, 2201)");
        
        query.exec("INSERT INTO tweet_topics VALUES (6,'2013-01-02 00:00:00',2200,null,'0:0.1 3:0.5','bla asdf')");
        query.exec("INSERT INTO tweet_topics VALUES (7,'2013-01-02 00:05:00',2201,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (9,'2013-01-02 00:08:00',2201,null,'0:0.1 1:0.5','bla brasil')");
        query.exec("INSERT INTO tweet_topics VALUES (10,'2013-01-02 00:10:00',2200,null,'0:0.1 1:0.5','bla brasil')");
        
        // Noise data
        query.exec("INSERT INTO tweet_topics VALUES (8,'2013-01-02 00:05:00',2202,null,'0:0.1 2:0.5','bla usp')");
        query.exec("INSERT INTO tweet_topics VALUES (10,'2013-01-02 00:15:00',2200,null,'0:0.1 1:0.5','bla brasil')");
    }
    
    TwitterUser user;
};


TEST_F(TwitterUserTest, TwitterUserIsCreatedWithTheGivenId)
{
    ASSERT_EQ(user.getUserId(), USER_ID);
}

TEST_F(TwitterUserTest, ProfileLoading)
{
    user.loadProfile(startProfile, endProfile);
    auto profile = user.getProfile();
    
    ASSERT_NEAR(profile.find("0")->second, 0.3, 0.01);
    ASSERT_NEAR(profile.find("1")->second, 0.5, 0.01);
    ASSERT_NEAR(profile.find("2")->second, 0.5, 0.01);
    ASSERT_NEAR(profile.find("3")->second, 0.5, 0.01);
}

TEST_F(TwitterUserTest, GetCandidatesTweetsOnlyReturnsTweetsPublishedByFollowedUsersCreatedInTheGivenInterval)
{
    auto candidates = user.getCandidates(startCandidates, endCandidates);
    
    ASSERT_EQ(candidates.size(), 4);
    ASSERT_EQ(candidates.at(0).getTweetId(), 6);
    ASSERT_EQ(candidates.at(1).getTweetId(), 7);
    ASSERT_EQ(candidates.at(2).getTweetId(), 9);
    ASSERT_EQ(candidates.at(3).getTweetId(), 10);
};