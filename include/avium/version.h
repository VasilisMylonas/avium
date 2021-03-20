#ifndef AVIUM_VERSION_H
#define AVIUM_VERSION_H

#include "avium/prologue.h"

// A type representing a software version in the format: major.minor.patch-tag
typedef struct AvmVersion* AvmVersion;

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 * @param tag The version tag (a, b, etc).
 *
 * @return AvmVersion The created AvmVersion.
 */
AVMAPI AvmVersion AvmVersionFrom(uint major, uint minor, uint patch, char tag);

/**
 * @brief Determines whether 2 AvmVersion instances are compatible (whether the
 * 2 major numbers are equal).
 *
 * @param self The first AvmVersion instance.
 * @param other The second AvmVersion instance.
 *
 * @return true The versions are compatible.
 * @return false The versions are not compatible.
 */
AVMAPI bool AvmVersionIsCompatible(AvmVersion self, AvmVersion other);

/**
 * @brief Gets the major number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The major number.
 */
AVMAPI uint AvmVersionGetMajor(AvmVersion self);

/**
 * @brief Gets the minor number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The minor number.
 */
AVMAPI uint AvmVersionGetMinor(AvmVersion self);

/**
 * @brief Gets the patch number of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return uint The patch number.
 */
AVMAPI uint AvmVersionGetPatch(AvmVersion self);

/**
 * @brief Gets the version tag of an AvmVersion instance.
 *
 * @param self The AvmVersion instance.
 * @return char The version tag number.
 */
AVMAPI char AvmVersionGetTag(AvmVersion self);

#endif  // AVIUM_VERSION_H
