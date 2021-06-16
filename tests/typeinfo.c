#include <avium/string.h>
#include <avium/typeinfo.h>

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

    AvmEquatable* e = cast(AvmEquatable, &p1);

    AvmPrintf("%t\n", AvmEquals(e, &p2));
}
