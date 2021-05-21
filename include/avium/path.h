#ifndef AVIUM_PATH_H
#define AVIUM_PATH_H

#include "avium/types.h"

AVMAPI char AvmPathGetSeparator(void);

AVMAPI bool AvmPathHasExtension(str path);
AVMAPI bool AvmPathIsRooted(str path);

AVMAPI AvmString AvmPathGetFileName(str path);
AVMAPI AvmString AvmPathGetExtension(str path);
AVMAPI AvmString AvmPathGetDirName(str path);

// AVMAPI str AvmPathGetHomeDir(void);
// AVMAPI str AvmPathGetTempDir(void);
// AVMAPI str AvmPathGetRandomFileName(void);

AVMAPI AvmString AvmPathGetFullPath(str path);

AVMAPI AvmString AvmPathCombine(size_t length, str paths[]);
AVMAPI AvmString AvmPathCombine2(str path1, str path2);
AVMAPI AvmString AvmPathCombine3(str path1, str path2, str path3);
AVMAPI AvmString AvmPathJoin(size_t count, ...);
AVMAPI AvmString AvmPathVJoin(size_t count, va_list args);

#endif // AVIUM_PATH_H
