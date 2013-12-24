#ifndef PROFILENOTLOADEDERROR_H
#define PROFILENOTLOADEDERROR_H

#include <exception>

class ProfileNotLoadedError : public std::exception
{
public:
    ProfileNotLoadedError()
    {
    }
    
    virtual const char* what()
    {
        return "Some operation TwitterUser required the profile to be loaded and it wasn't.";
    }
};

#endif // PROFILENOTLOADEDERROR_H