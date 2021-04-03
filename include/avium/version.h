#ifndef AVIUM_VERSION_H
#define AVIUM_VERSION_H

#include "avium/types.h"
#include "avium/exports.h"

/// A type representing a software version in the format: N.N.N-c
AVM_CLASS(AvmVersion, object, {
    ushort Major;
    ushort Minor;
    ushort Patch;
    char Tag;
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
 * @brief Return an AvmVersion indicating the current runtime version.
 *
 * @return The current runtime version.
 */
AVMAPI AvmVersion AvmRuntimeGetVersion();

static_assert_s(sizeof(AvmVersion) == AVM_VERSION_SIZE);

#endif  // AVIUM_VERSION_H
