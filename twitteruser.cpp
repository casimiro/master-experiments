#include "twitteruser.h"

namespace casimiro {

TwitterUser::TwitterUser(long int _userId):
    m_userId(_userId)
{
}

long int TwitterUser::getUserId() const
{
    return m_userId;
}

void TwitterUser::loadProfile(const session& _conParams, const ptime& _start, const ptime& _end)
{
    m_profile["0"] = 0.1;
    m_profile["1"] = 0.5;
}

const StringFloatMap& TwitterUser::getProfile() const
{
    return m_profile;
}

}