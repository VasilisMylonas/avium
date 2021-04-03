#ifndef AVIUM_COLLECTION_H
#define AVIUM_COLLECTION_H

#include "avium/types.h"

typedef object AvmCollection;

/**
 * @brief Gets the length of a collection.
 *
 * This function tries to use the FUNC_GET_LENGTH virtual function entry to
 * get the length of the collection. If no such virtual function is available
 * then this function traps.
 *
 * @param self The collection.
 * @return size_t The length of the collection.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI size_t AvmCollectionGetLength(AvmCollection self);

/**
 * @brief Gets the capacity of a collection.
 *
 * This function tries to use the FUNC_GET_CAPACITY virtual function entry to
 * get the capacity of the collection. If no such virtual function is available
 * then this function traps.
 *
 * @param self The collection.
 * @return size_t The capacity of the collection.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI size_t AvmCollectionGetCapacity(AvmCollection self);

#endif  // AVIUM_COLLECTION_H
