#ifndef AVIUM_RUNTIME_H
#define AVIUM_RUNTIME_H

#include "avium/types.h"

AVMAPI void AvmRuntimeEnableExceptions(void);
AVMAPI void AvmRuntimeDisableExceptions(void);

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to print.
 */
#define AvmPanic(message) AvmPanicEx(message, __func__, __FILE__, __LINE__)

/**
 * @brief Aborts execution, printing a message and location information.
 *
 * @param message The message to be printed.
 * @param function The function name.
 * @param file The file name.
 * @param line The line number.
 *
 * @return never This function never returns.
 */
AVMAPI never AvmPanicEx(str message, str function, str file, uint line);

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

enum {
    FUNC_DTOR = 0,
    FUNC_GET_LENGTH,
    FUNC_GET_CAPACITY,
    FUNC_TO_STRING,
    FUNC_CLONE,
    FUNC_EQUALS,
    FUNC_READ,
    FUNC_WRITE,
    FUNC_READ_STRING,
    FUNC_WRITE_STRING,
};

#define AVM_GET_TYPE(T)  AVM_GET_TYPE_(T)
#define AVM_TYPE(T, ...) AVM_TYPE_(T, __VA_ARGS__)

struct AvmType {
    const AvmFunction* _vptr;
    str _name;
    size_t _size;
};

/**
 * @brief Gets the name of a type.
 *
 * @param self The type.
 * @return str The type's name.
 */
AVMAPI str AvmTypeGetName(const AvmType* self);

/**
 * @brief Gets the size of a type.
 *
 * @param self The type.
 * @return size_t The type's size.
 */
AVMAPI size_t AvmTypeGetSize(const AvmType* self);

#define AVM_TYPE_(T, ...)                                \
    static const AvmType _##T##Type = {                  \
        ._vptr = (AvmFunction[AVM_VFT_SIZE])__VA_ARGS__, \
        ._name = #T,                                     \
        ._size = sizeof(T),                              \
    }

#define AVM_GET_TYPE_(T) &_##T##Type

#endif  // AVIUM_RUNTIME_H
