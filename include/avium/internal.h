#include "avium/core.h"

#define AVM_VMT_SIZE 32

struct Type {
    function_t* vptr;
    const char8_t* name;
    size_t size;
};

#define TYPE(T, vptr) \
    { vptr, #T, sizeof(struct T), }

enum {
    FUNC_DTOR = 0,
    FUNC_GET_LENGTH,
    FUNC_GET_CAPACITY,
    FUNC_TO_STRING,
    FUNC_CLONE,
    FUNC_EQ,
    FUNC_READ,
    FUNC_WRITE,
    FUNC_READ_STRING,
    FUNC_WRITE_STRING,
};
