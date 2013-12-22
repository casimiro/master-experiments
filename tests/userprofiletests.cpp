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

class TwitterUserTest: public Test {
protected:
    TwitterUserTest():user(USER_ID)
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("casimiro.db.sqlite");
        db.open();
    }
    
    virtual void SetUp()
    {
        user = TwitterUser(USER_ID);
        createDbStructure();
        persistProfileData();
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
        std::ifstream file("setup.sql");
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        QSqlQuery query;
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
    
    TwitterUser user;
};


TEST_F(TwitterUserTest, TwitterUserCreation)
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

TEST_F(TwitterUserTest, GetCandidatesTweets)
{
    auto candidates = user.getCandidates(startCandidates, endCandidates);
    
    ASSERT_EQ(candidates.size(), 4);
}