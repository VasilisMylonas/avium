#include "avium/path.h"

#include "avium/private/basename.h"
#include "avium/private/constants.h"
#include "avium/private/realpath.h"
#include "avium/string.h"
#include "avium/testing.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char AvmPathGetSeparator(void)
{
    return AVM_PATH_SEPARATOR;
}

char AvmPathGetAltSeparator(void)
{
    return AVM_PATH_ALT_SEPARATOR;
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

bool AvmPathIsAbsolute(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return path[0] == AvmPathGetSeparator() ||
           path[0] == AvmPathGetAltSeparator();
}

bool AvmPathIsRelative(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return path[0] == '.' && (path[1] == AvmPathGetSeparator() ||
                              path[1] == AvmPathGetAltSeparator());
}

bool AvmPathIsValid(str path)
{
    pre
    {
        assert(path != NULL);
    }

    return AVM_PATH_IS_VALID(path);
}

bool AvmPathIsDir(str path)
{
    pre
    {
        assert(path != NULL);
    }

    const uint length = strlen(path);
    const char sep = AvmPathGetSeparator();
    const char alt = AvmPathGetAltSeparator();

    return path[length - 1] == sep || path[length - 1] == alt;
}

AvmString AvmPathGetName(str path)
{
    pre
    {
        assert(path != NULL);
    }

    AvmString s = AvmStringFrom(AvmBasename(path));

    if (AvmPathIsDir(path))
    {
        s._length--;
    }

    return s;
}

AvmString AvmPathGetExtension(str path)
{
    pre
    {
        assert(path != NULL);
    }

    AvmString name = AvmPathGetName(path);
    uint index = AvmStringLastIndexOf(&name, '.');

    if (index == AvmInvalid)
    {
        return AvmStringNew(0);
    }

    const uint length = AvmStringGetLength(&name) - index - 1;
    const str buffer = AvmStringGetBuffer(&name) + index + 1; // Skip the dot.

    return AvmStringFromChars(length, buffer);
}

AvmString AvmPathGetParent(str path)
{
    pre
    {
        assert(path != NULL);
    }

    AvmString s = AvmStringFrom(path);

    if (s._length == AVM_PATH_ROOT_LENGTH)
    {
        return s;
    }

    const char sep = AvmPathGetSeparator();

    AVM_PATH_NORMALIZE(&s);

    uint index = AvmStringLastIndexOf(&s, sep);

    if (index == AvmInvalid)
    {
        return s;
    }

    s._length = index + 1;

    return s;
}

AvmString AvmPathGetFullPath(str path)
{
    pre
    {
        assert(path != NULL);
    }

    char buffer[AVM_PATH_MAX] = {0};
    if (realpath(path, buffer) != NULL)
    {
        return AvmStringFrom(buffer);
    }

    // TODO: error
    throw(AvmErrorNew("TODO"));
}

AvmString AvmPathGetTempDir(void)
{
    return AvmStringFrom(P_tmpdir);
}

AvmString AvmPathGetHomeDir(void)
{
    str home = getenv(HomeEnvVar);
    if (home != NULL)
    {
        return AvmStringFrom(home);
    }

    throw(AvmErrorNew(HomeDirNotDeterminedError));
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
        prev = temp;
    }

    return prev;
}

AvmString AvmPathCombine2(str path1, str path2)
{
    pre
    {
        assert(path1 != NULL);
        assert(path2 != NULL);
    }

    const char sep = AvmPathGetSeparator();
    const char alt = AvmPathGetAltSeparator();
    const size_t length1 = strlen(path1);
    const size_t length2 = strlen(path2);

    // If a string is empty then just return the other.
    if (length1 == 0)
    {
        return AvmStringFrom(path2);
    }

    if (length2 == 0)
    {
        return AvmStringFrom(path1);
    }

    // Preallocate with the total length.
    AvmString temp = AvmStringNew(length1 + length2);

    // Start with the first path.
    AvmStringPush(&temp, path1);

    // Add the separator if needed.
    if (path1[length1 - 1] != sep)
    {
        AvmStringPush(&temp, sep);
    }

    uint offset = 0;

    if (path2[0] == '.' && (path2[1] == sep || path2[1] == alt))
    {
        // If the second path starts with ./
        offset = 2;
    }
    else if (path2[0] == sep || path2[0] == alt)
    {
        // If the second path starts with / ignore it.
        offset = 1;
    }

    AvmStringPush(&temp, path2 + offset);

    AVM_PATH_NORMALIZE(&temp);

    return temp;
}
