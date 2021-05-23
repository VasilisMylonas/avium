#include "avium/path.h"

#include "avium/basename.h"
#include "avium/error.h"
#include "avium/private/errors.h"
#include "avium/private/resources.h"
#include "avium/string.h"
#include "avium/testing.h"
#include <string.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef AVM_WIN32
#include <shlwapi.h>
#endif

char AvmPathGetSeparator(void)
{
#ifdef AVM_WIN32
    return '\\';
#else
    return '/';
#endif
}

bool AvmPathHasExtension(str path)
{
    pre
    {
        assert(path != NULL);
    }

    str name = AvmBasename(path);
    return strrchr(name, '.') != NULL;
}

bool AvmPathIsRooted(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return path[0] == AvmPathGetSeparator();
}

bool AvmPathIsValid(str path)
{
    pre
    {
        assert(path != NULL);
    }

#ifdef AVM_WIN32
    return PathFileExistsA(path);
#else
    return strstr(path, "//") == NULL;
#endif
}

AvmString AvmPathGetFileName(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return AvmStringFrom(AvmBasename(path));
}

AvmString AvmPathGetExtension(str path)
{
    pre
    {
        assert(path != NULL);
    }

    AvmString name = AvmPathGetFileName(path);
    uint index = AvmStringLastIndexOf(&name, '.');

    if (index == AvmInvalid)
    {
        AvmObjectDestroy(&name);
        return AvmStringNew(0);
    }

    const uint length = AvmStringGetLength(&name) - index - 1;
    const str buffer = AvmStringGetBuffer(&name) + index + 1; // Skip the dot.

    return AvmStringFromChars(length, buffer);
}

AvmString AvmPathGetDirName(str path)
{
    pre
    {
        assert(path != NULL);
    }

    char* ptr = strrchr(path, AvmPathGetSeparator());

    if (ptr == NULL)
    {
        return AvmStringNew(0);
    }

    const uint length = (uint)(ptr - path) + 1;

    return AvmStringFromChars(length, path);
}

AvmString AvmPathGetFullPath(str path)
{
    pre
    {
        assert(path != NULL);
    }

#ifdef AVM_WIN32
    char buffer[_MAX_PATH] = {0};
    if (_fullpath(buffer, path, _MAX_PATH) != NULL)
#else
    char buffer[PATH_MAX] = {0};
    if (realpath(path, buffer) != NULL)
#endif
    {
        return AvmStringFrom(buffer);
    }

    // TODO: error
    AvmPanic("TODO");
}

str AvmPathGetTempDir(void)
{
    return P_tmpdir;
}

str AvmPathGetHomeDir(void)
{
    str home = getenv(HomeEnvVar);
    if (home != NULL)
    {
        return home;
    }

    AvmPanic(HomeDirNotDeterminedError);
}

AvmString AvmPathCombine(uint length, str paths[])
{
    pre
    {
        assert(length != 0);
        assert(paths != NULL);
    }

    if (length == 1)
    {
        return AvmStringFrom(paths[0]);
    }

    AvmString prev = AvmStringFrom(paths[0]);

    for (size_t i = 1; i < length; i++)
    {
        // TODO: There sure are more efficient ways of doing this.

        // Combine the previous and the current paths.
        AvmString temp = AvmPathCombine2(AvmStringGetBuffer(&prev), paths[i]);

        // Replace the old string.
        AvmObjectDestroy(&prev);
        prev = temp;
    }

    return prev;
}

AvmString AvmPathCombine3(str path1, str path2, str path3)
{
    pre
    {
        assert(path1 != NULL);
        assert(path2 != NULL);
        assert(path3 != NULL);
    }

    AvmString temp = AvmPathCombine2(path1, path2);
    AvmString ret = AvmPathCombine2(AvmStringGetBuffer(&temp), path3);
    AvmObjectDestroy(&temp);
    return ret;
}

AvmString AvmPathCombine2(str path1, str path2)
{
    pre
    {
        assert(path1 != NULL);
        assert(path2 != NULL);
    }

    const char sep = AvmPathGetSeparator();
    const size_t length1 = strlen(path1);
    const size_t length2 = strlen(path2);

    if (length1 == 0)
    {
        return AvmStringFrom(path2);
    }

    if (length2 == 0)
    {
        return AvmStringFrom(path1);
    }

    AvmString temp = AvmStringNew(length1 + length2);

    AvmStringPush(&temp, path1);

    if (path1[length1 - 1] != sep)
    {
        AvmStringPush(&temp, sep);
    }

    if (path2[0] == '.' && path2[1] == sep)
        AvmStringPush(&temp, path2 + 2);
    else if (path2[0] == sep)
        AvmStringPush(&temp, path2 + 1);
    else
        AvmStringPush(&temp, path2);

    return temp;
}
