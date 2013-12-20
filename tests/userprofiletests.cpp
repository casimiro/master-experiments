#include <gmock/gmock.h>

class TwitterUser {
public:
    TwitterUser(long _userId):
        m_userId(_userId)
    {
    }
    
    ~TwitterUser() {}
    
    virtual long getUserId() const { return m_userId; }
    
private:
    long m_userId;
};

long USER_ID = 2256;

TEST(TwitterUser, TwitterUserCreation)
{
    TwitterUser userProfile(USER_ID);
    ASSERT_EQ(userProfile.getUserId(), USER_ID);
}