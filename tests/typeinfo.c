#include "avium/core.h"

// #undef AVM_CLASS
// #undef AVM_TI_NAME
// #undef AVM_VT_NAME
// #undef AVM_MTI_NAME
// #undef AVM_PTI_NAME
// #undef AVM_CTI_NAME
// #undef AVM_INTERFACE
// #include "avium/typeinfo-new.h"

// typedef uint wchar;

// AVM_CLASS(AvmWideString, object, {
//     struct
//     {
//         uint length;
//         uint capacity;
//         wchar* buffer;
//     } _private;
// });

// AVM_CLASS_TYPE(object, object, {NULL});

// AVM_INTERFACE(AvmLengthable,
//               {
//                   AvmLengthableGetLength,
//               });

// static uint AvmWideStringGetLength(const AvmWideString* self)
// {
//     return self->_private.length;
// }

// AVM_IMPLEMENT(
//     AvmLengthable,
//     AvmWideString,
//     {
//         [AvmLengthableGetLength] = (AvmCallback)AvmWideStringGetLength,
//     });

// AVM_INTERFACES(AvmWideString, {[0] = &_Impl_AvmLengthable_AvmWideString});

// AVM_MEMBERS(AvmWideString, AVM_MEMBERS_DEFAULT);

// AVM_CLASS_TYPE_EX(AvmWideString, object, AVM_VTABLE_DEFAULT);

// implement AvmLengthable for AvmWideString {
//     AvmLengthableGetLength = AvmWideStringGetLength,
// };

// uint AvmGetLength(AvmLengthable* self)
// {
//     return icall(AvmLengthableGetLength, uint, object)(self->Value);
// }

// AvmWideString AvmWideStringWithLength(uint length)
// {
//     AvmWideString s = {
//         .__type = typeid(AvmWideString),
//         ._private =
//             {
//                 .length = length,
//             },
//     };

//     return s;
// }

void Main()
{
    // AvmWideString s = AvmWideStringWithLength(20);

    // AvmLengthable l = QueryInterface(&s, AvmLengthable);
    // AvmPrintf("Length! %u\n", AvmGetLength(&l));
    AvmPrintf("Hello!\n");
}
