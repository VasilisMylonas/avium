#ifndef AVIUM_PATH_H
#define AVIUM_PATH_H

#include "avium/types.h"

AVMAPI bool AvmPathHasExtension(str path);
AVMAPI bool AvmPathIsRooted(str path);

AVMAPI str AvmPathGetExtension(str path);
AVMAPI str AvmPathGetFileName(str path);
AVMAPI str AvmPathGetPureFileName(str path);
AVMAPI str AvmPathGetDirName(str path);

AVMAPI str AvmPathGetHomeDir(str path);
AVMAPI str AvmPathGetTempDir(str path);
AVMAPI str AvmPathGetTempFile(str path);
AVMAPI str AvmPathGetRandomFileName(str path);

AVMAPI str AvmPathGetFullPath(str path);

AVMAPI str AvmPathCombine(size_t length, str paths[]);
AVMAPI str AvmPathCombine2(str path1, str path2);
AVMAPI str AvmPathCombine3(str path1, str path2, str path3);
AVMAPI str AvmPathJoin(size_t count, ...);
AVMAPI str AvmPathVJoin(size_t count, va_list args);

#endif  // AVIUM_PATH_H
