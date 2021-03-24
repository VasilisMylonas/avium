#ifndef AVIUM_VERSION_H
#define AVIUM_VERSION_H

#include "avium/prologue.h"

/// A type representing a software version in the format: N.N.N-c
AVM_CLASS(AvmVersion, object, {
    ushort major;
    ushort minor;
    ushort patch;
    char tag;
});

/**
 * @brief Creates a new AvmVersion instance.
 *
 * @param major The version major number (incremented at breaking changes).
 * @param minor The version minor number (incremented at non-breaking changes).
 * @param patch The version patch number (incremented at bug fixes).
 * @param tag The version tag (a, b, etc).
 *
 * @return The created instance.
 */
AVMAPI AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch,
                                 char tag);

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
AVMAPI bool AvmVersionIsCompatible(AvmVersion* self, AvmVersion* other);

static_assert(sizeof(AvmVersion) == 16, "");

#endif  // AVIUM_VERSION_H
