/*
 *
 */

#include "profilenotloadederror.h"

ProfileNotLoadedError::ProfileNotLoadedError()
{

}

const char* ProfileNotLoadedError::what()
{
    return "Some operation TwitterUser required the profile to be loaded and it wasn't.";
}
