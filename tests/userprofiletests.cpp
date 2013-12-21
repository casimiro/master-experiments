#include <gmock/gmock.h> 
#include <boost/date_time.hpp>
#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include "twitteruser.h"

using namespace ::testing;
using namespace boost::posix_time;
using namespace soci;
using namespace casimiro;

long USER_ID = 2256;
ptime startProfile = time_from_string("2013-01-01 00:00:00");
ptime endProfile = time_from_string("2013-01-01 00:00:00");

class TwitterUserTest: public Test {
protected:
    TwitterUserTest():user(USER_ID)
    {
        con.close();
    }
    
    virtual void SetUp()
    {
        user = TwitterUser(USER_ID);
        con.open(sqlite3, "test.db");
    }
    
    TwitterUser user;
    session con;
};


TEST_F(TwitterUserTest, TwitterUserCreation)
{
    ASSERT_EQ(user.getUserId(), USER_ID);
}

TEST_F(TwitterUserTest, ProfileLoading)
{
    user.loadProfile(con, startProfile, endProfile);
    auto profile = user.getProfile();
    
    ASSERT_NEAR(profile.find("0")->second, 0.1, 0.1);
    ASSERT_NEAR(profile.find("1")->second, 0.5, 0.1);
}