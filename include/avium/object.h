#ifndef AVIUM_OBJECT_H
#define AVIUM_OBJECT_H

#include "avium/prologue.h"
#include "avium/string.h"

/**
 * @brief Creates a scope in which an object will be available and be destroyed
 *        at scope exit.
 *
 * @param T The type of the object.
 * @param name The name of the variable.
 * @param init The object initializer.
 */
#define defer(T, name, init) \
    for (T name = init; name != NULL; AvmObjectDestroy(name), name = NULL)

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

/**
 * @brief Compares two objects for equality.
 *
 * This function tries to use the FUNC_EQ virtual function entry to compare
 * for equality. If no such virtual function is available then memcmp is used.
 *
 * @param lhs The first object.
 * @param rhs The second object.
 *
 * @return true The two objects are equal.
 * @return false The two objects are not equal.
 */
AVMAPI bool AvmObjectEquals(object lhs, object rhs);

/**
 * @brief Destroys an object and deallocates its memory.
 *
 * This function tries to use the FUNC_DTOR virtual function entry to destroy
 * the object. If no such virtual function is available then free is used.
 *
 * @param self The object.
 */
AVMAPI void AvmObjectDestroy(object self);

/**
 * @brief Clones an object, creating an exact copy.
 *
 * This function tries to use the FUNC_CLONE virtual function entry to clone
 * the object. If no such virtual function is available then a combination of
 * malloc and memcpy is used.
 *
 * @param self The object.
 * @return object The cloned object.
 */
AVMAPI object AvmObjectClone(object self);

/**
 * @brief Creates a string representation of an object.
 *
 * This function tries to use the FUNC_TO_STRING virtual function entry to
 * create a string representation of the object. If no such virtual function
 * is available then this function traps.
 *
 * @param self The object.
 * @return AvmString The string representation of the object.
 *
 * @see AvmVirtualFunctionTrap
 */
AVMAPI AvmString AvmObjectToString(object self);

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
