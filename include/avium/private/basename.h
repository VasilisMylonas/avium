#ifndef AVIUM_PRIVATE_BASENAME_H
#define AVIUM_PRIVATE_BASENAME_H

#include "avium/testing.h"
#include <string.h>

static str AvmBasename(str path)
{
    pre
    {
        assert(path != NULL);
    }

#ifdef AVM_WIN32
    str name = strrchr(path, '\\') + 1;

    if (name == NULL)
    {
        name = strrchr(path, '/') + 1;
    }
#else
    str name = strrchr(path, '/') + 1;
#endif

    if (name == NULL)
    {
        name = path;
    }

    return name;
}

#endif // AVIUM_PRIVATE_BASENAME_H
