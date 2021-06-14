#include <avium/string.h>
#include <avium/typeinfo.h>

// AVM_INTERFACE(AvmStringable,
//               {
//                   AvmStringableToString,
//               });

// AvmString AvmPointToString(const AvmPoint* self)
// {
//     pre
//     {
//         assert(self != NULL);
//     }

//     return AvmStringFormat("(%f, %f)", self->X, self->Y);
// }

// AVM_IMPLEMENT(AvmStringable,
//               AvmPoint,
//               {
//                   [AvmStringableToString] = (AvmCallback)AvmPointToString,
//               });

// AVM_INTERFACES(AvmPoint,
//                {
//                    interfaceid(AvmStringable, AvmPoint),
//                });

// AVM_MEMBERS(AvmPoint, AVM_MEMBERS_DEFAULT);

AVM_CLASS(AvmPoint, object, {
    float X;
    float Y;
});

AVM_CLASS_TYPE(AvmPoint, object, AVM_VTABLE_DEFAULT);

AvmPoint AvmPointNew(float x, float y)
{
    return (AvmPoint){
        .__type = typeid(AvmPoint),
        .X = x,
        .Y = y,
    };
}

static_assert(sizeof(AvmType) == 24, "");
static_assert(sizeof(AvmClass) == 64, "");
static_assert(sizeof(AvmFunction) == 48, "");
static_assert(sizeof(AvmMember) == 32, "");
static_assert(sizeof(AvmEnum) == 32, "");
static_assert(sizeof(AvmInterface) == 32, "");

void Main()
{
    AvmPoint p1 = AvmPointNew(5, 2.64);
    AvmPoint p2 = AvmPointNew(5, 2.64);

    AvmEquatable e = AvmQueryInterface(&p1, AvmEquatable);

    const AvmInterface* c =
        AvmClassGetInterface(AvmObjectGetType(&p1), "AvmEquatable");

    AvmPrintf("%t\n", AvmEquals(&e, &p2));
}
