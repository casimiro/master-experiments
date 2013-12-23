#ifndef PROFILENOTLOADEDERROR_H
#define PROFILENOTLOADEDERROR_H

#include <exception>

class ProfileNotLoadedError :  std::exception
{
public:
    ProfileNotLoadedError();
    virtual const char* what();
};

#endif // PROFILENOTLOADEDERROR_H
