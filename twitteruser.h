#include <boost/date_time.hpp>
#include <soci/soci.h>
#include <map>
#include <string>

using namespace boost::posix_time;
using namespace soci;

#ifndef TWITTERUSER_H
#define TWITTERUSER_H

namespace casimiro {

typedef std::map<std::string, float> StringFloatMap;    

class TwitterUser {
public:
    TwitterUser(long _userId);
    ~TwitterUser() {}
    
    virtual long getUserId() const;
    
    virtual void loadProfile(const session &_conParams, const ptime &_start, const ptime &_end);
    
    virtual const StringFloatMap& getProfile() const;
    
private:
    long m_userId;
    StringFloatMap m_profile;
};

}
#endif // TWITTERUSER_H
