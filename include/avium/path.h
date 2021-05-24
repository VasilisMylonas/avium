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
 */

#ifndef AVIUM_PATH_H
#define AVIUM_PATH_H

#include "avium/types.h"

/**
 * @brief Gets the character used as a path separator.
 *
 * @return The path separator.
 */
AVMAPI char AvmPathGetSeparator(void);

/**
 * @brief Gets the alternative character used as a path separator.
 *
 * @return The alternative path separator.
 */
AVMAPI char AvmPathGetAltSeparator(void);

/**
 * @brief Determines whether a path has an extension.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return true if the path has an extension, otherwise false.
 */
AVMAPI bool AvmPathHasExtension(str path);

/**
 * @brief Determines whether a path is absolute.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return true if the path is absolute, otherwise false.
 */
AVMAPI bool AvmPathIsAbsolute(str path);

/**
 * @brief Determines whether a path is relative.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return true if the path is relative, otherwise false.
 */
AVMAPI bool AvmPathIsRelative(str path);

/**
 * @brief Determines whether a path is valid.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return true if the path is valid, otherwise false.
 */
AVMAPI bool AvmPathIsValid(str path);

/**
 * @brief Returns the name of the file or directory represented by a given path.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return The file or directory name.
 */
AVMAPI AvmString AvmPathGetName(str path);

/**
 * @brief Returns the file extension component of a path, or an empty string if
 *        it does not represent a file.
 *
 * @pre Parameter @p path must be not null.
 *
 * @param path The path.
 * @return The file extension.
 */
AVMAPI AvmString AvmPathGetExtension(str path);

AVMAPI AvmString AvmPathGetParent(str path);

AVMAPI bool AvmPathIsDir(str path);

AVMAPI str AvmPathGetHomeDir(void);
AVMAPI str AvmPathGetTempDir(void);

AVMAPI AvmString AvmPathGetFullPath(str path);

AVMAPI AvmString AvmPathCombine(uint length, str paths[]);
#define AvmPathCombineV(...) AvmPathCombine(AVM_VA_ARGS(str, __VA_ARGS__))

AVMAPI AvmString AvmPathCombine2(str path1, str path2);
AVMAPI AvmString AvmPathCombine3(str path1, str path2, str path3);

#endif // AVIUM_PATH_H
