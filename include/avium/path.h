#ifndef AVIUM_PATH_H
#define AVIUM_PATH_H

#include "avium/types.h"

AVMAPI char AvmPathGetSeparator(void);

AVMAPI bool AvmPathHasExtension(str path);
AVMAPI bool AvmPathIsRooted(str path);
AVMAPI bool AvmPathIsValid(str path);

AVMAPI AvmString AvmPathGetFileName(str path);
AVMAPI AvmString AvmPathGetExtension(str path);
AVMAPI AvmString AvmPathGetDirName(str path);

AVMAPI str AvmPathGetHomeDir(void);
AVMAPI str AvmPathGetTempDir(void);

AVMAPI AvmString AvmPathGetFullPath(str path);

AVMAPI AvmString AvmPathCombine(uint length, str paths[]);
#define AvmPathCombineV(...) AvmPathCombine(AVM_VA_ARGS(str, __VA_ARGS__))

AVMAPI AvmString AvmPathCombine2(str path1, str path2);
AVMAPI AvmString AvmPathCombine3(str path1, str path2, str path3);

#endif // AVIUM_PATH_H
