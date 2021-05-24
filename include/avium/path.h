/**
 * @file avium/path.h
 * @author Vasilis Mylonas <vasilismylonas@protonmail.com>
 * @brief Filesystem path utilities.
 * @version 0.1
 * @date 2021-05-24
 *
 * @copyright Copyright (c) 2021 Vasilis Mylonas
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 *
 */

#ifndef AVIUM_PATH_H
#define AVIUM_PATH_H

#include "avium/types.h"

AVMAPI char AvmPathGetSeparator(void);
AVMAPI char AvmPathGetAltSeparator(void);

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
