#ifndef AVIUM_PRIVATE_REALPATH_H
#define AVIUM_PRIVATE_REALPATH_H

#include "avium/core.h"

#define AVM_PATH_ALT_SEPARATOR '/'

#ifdef AVM_WIN32
#include <shlwapi.h> // _fullpath
#include <stdlib.h>  //  _MAX_PATH
#define AVM_PATH_SEPARATOR   '\\'
#define AVM_PATH_MAX         _MAX_PATH
#define AVM_PATH_ROOT_LENGTH 3

#define AVM_PATH_NORMALIZE(string)                                             \
    AvmStringReplaceAll(string, AVM_PATH_ALT_SEPARATOR, AVM_PATH_SEPARATOR)

#define AVM_PATH_IS_VALID(path) PathFileExistsA(path)

static char* realpath(str name, char* resolved)
{
    return _fullpath(buffer, path, AVM_PATH_MAX);
}
#else
#include <limits.h> // PATH_MAX
#include <stdlib.h> // realpath
#define AVM_PATH_SEPARATOR   '/'
#define AVM_PATH_MAX         PATH_MAX
#define AVM_PATH_ROOT_LENGTH 1

#define AVM_PATH_NORMALIZE(string) ((void)0)
#define AVM_PATH_IS_VALID(path)    strstr(path, "//") == NULL
#endif

#endif // AVIUM_PRIVATE_REALPATH_H
