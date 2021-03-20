#include "avium/prologue.h"

static const str SelfNullMsg = "Parameter `self` was `NULL`.";
static const str ContentsNullMsg = "Parameter `contents` was `NULL`.";
static const str SourceNullMsg = "Parameter `source` was `NULL`.";
static const str DestNullMsg = "Parameter `destination` was `NULL`.";
static const str FormatNullMsg = "Parameter `format` was `NULL`.";
static const str ArgsNullMsg = "Parameter `args` was `NULL`.";
static const str HandleNullMsg = "Parameter `handle` was `NULL`.";
static const str BaseOutOfRangeMsg = "Parameter `base` was out of range`.";
static const str StringNullMsg = "Parameter `string` was `NULL`.";
static const str OtherNullMsg = "Parameter `other` was `NULL`.";

struct AvmType {
    const AvmFunction* vptr;
    str name;
    size_t size;
};

#define TYPE(T, ...)                                         \
    static const struct AvmType T##Type = {                  \
        .vptr = (AvmFunction[AVM_VTABLE_SIZE]){__VA_ARGS__}, \
        .name = #T,                                          \
        .size = sizeof(struct T),                            \
    }

#define GET_TYPE(T) (AvmType) & T##Type

enum {
    AVM_VTABLE_SIZE = 32,
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
