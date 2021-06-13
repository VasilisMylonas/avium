#ifndef AVIUM_PRIVATE_VIRTUAL_H
#define AVIUM_PRIVATE_VIRTUAL_H

#define VIRTUAL_CALL_(TReturn, E, ...)                                         \
    AvmCallback __virtualFunc =                                                \
        AvmClassGetCallback(AvmObjectGetType((object)self), E);                \
    return ((TReturn(*)())__virtualFunc)(__VA_ARGS__);

#ifdef AVM_GNU
#define VIRTUAL_CALL(TReturn, E, ...)                                          \
    _Pragma("GCC diagnostic push")                                             \
        _Pragma("GCC diagnostic ignored \"-Wpedantic\"")                       \
            VIRTUAL_CALL_(TReturn, E, __VA_ARGS__);                            \
    _Pragma("GCC diagnostic pop")
#else
#define VIRTUAL_CALL(TReturn, E, ...) VIRTUAL_CALL_(TReturn, E, __VA_ARGS__)
#endif

#endif // AVIUM_PRIVATE_VIRTUAL_H
