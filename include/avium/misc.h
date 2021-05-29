#ifndef AVIUM_MISC_H
#define AVIUM_MISC_H

/**
 * @brief Creates an array from a va_list.
 *
 * @pre Parameter @p N must be not zero.
 * @pre Parameter @p args must be not null.
 *
 * @param T The type of the array elements.
 * @param N The length of the array.
 * @param args The va_list.
 *
 * @return The created array.
 */
#define va_array(T, N, args)                                                   \
    (T*)__AvmVaListToArray(AvmAlloc(sizeof(T) * N), args, sizeof(T), N);

#define VA_LIST_TO_ARRAY_IMPL(T1, T2)                                          \
    for (size_t i = 0; i < length; i++)                                        \
    {                                                                          \
        ((T1*)stack)[i] = (T1)va_arg(args, T2);                                \
    }                                                                          \
                                                                               \
    return stack;

AVMAPI void* __AvmVaListToArray(void*, va_list, uint, uint);

void* __AvmVaListToArray(void* stack, va_list args, uint size, uint length)
{
    pre
    {
        assert(stack != NULL);
        assert(args != NULL);
        assert(size != 0);
        assert(length != 0);
    }

    switch (size)
    {
    case 1:
        VA_LIST_TO_ARRAY_IMPL(byte, uint);
    case 2:
        VA_LIST_TO_ARRAY_IMPL(ushort, uint);
    case 4:
        VA_LIST_TO_ARRAY_IMPL(uint, uint);
    case 8:
        VA_LIST_TO_ARRAY_IMPL(ulong, ulong);
    default:
        return NULL; // TODO: ERROR
    }
}

#endif // AVIUM_MISC_H
