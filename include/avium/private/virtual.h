#ifndef AVIUM_PRIVATE_VIRTUAL_H
#define AVIUM_PRIVATE_VIRTUAL_H

#define VIRTUAL_CALL_(TReturn, E, ...)                                         \
    AvmCallback __virtualFunc =                                                \
        AvmTypeGetCallback(AvmObjectGetType((object)self), E);                 \
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

#define VA_LIST_TO_ARRAY_IMPL(T1, T2)                                          \
    for (size_t i = 0; i < length; i++)                                        \
    {                                                                          \
        ((T1*)array)[i] = (T1)va_arg(args, T2);                                \
    }                                                                          \
                                                                               \
    return array;

#endif // AVIUM_PRIVATE_VIRTUAL_H
