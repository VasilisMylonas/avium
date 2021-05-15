#include "avium/basename.h"

#include <string.h>

#ifdef AVM_WIN32
const char AvmPathSeparator = '\\';
const char AvmPathAltSeparator = '/';
#else
const char AvmPathSeparator = '/';
const char AvmPathAltSeparator = '/';
#endif

str AvmBasename(str path) {
    str name = strrchr(path, AvmPathSeparator);

    if (name == NULL) {
        name = strrchr(path, AvmPathAltSeparator);
    }

    if (name == NULL) {
        name = path;
    }

    return name;
}