#ifndef AVIUM_OBJECT_H
#define AVIUM_OBJECT_H

#include "avium/prologue.h"

/// A type containing information about an object.
typedef struct AvmType* AvmType;

/**
 * @brief Gets the name of a type.
 *
 * @param self The type.
 * @return str The type's name.
 */
AVMAPI str AvmTypeGetName(AvmType self);

/**
 * @brief Gets the size of a type.
 *
 * @param self The type.
 * @return size_t The type's size.
 */
AVMAPI size_t AvmTypeGetSize(AvmType self);

/**
 * @brief Gets information about the type of an object.
 *
 * @param self The object.
 * @return AvmType The type information of the object.
 */
AVMAPI AvmType AvmObjectType(object self);

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

/**
 * @brief The trap function called when a virtual function is not implemented.
 *
 * @param function The function name.
 * @param type The object type.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmVirtualFunctionTrap(str function, AvmType type);

/**
 * @brief Copies memory from one block to another.
 *
 * This will copy length bytes from source to destination, but not more than
 * size.
 *
 * @param source The memory block to copy from.
 * @param length The length of the source buffer.
 * @param destination The memory block to copy to.
 * @param size The size of the destination buffer.
 */
AVMAPI void AvmMemCopy(byte* source, size_t length, byte* destination,
                       size_t size);

#endif  // AVIUM_OBJECT_H
