#include <avium/string.h>
#include <avium/typeinfo.h>

AVM_INTERFACE(AvmStringable,
              {
                  AvmStringableToString,
              });

AVM_CLASS(AvmPoint, object, {
    float X;
    float Y;
});

AvmString AvmPointToString(const AvmPoint* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat("(%f, %f)", self->X, self->Y);
}

AVM_IMPLEMENT(AvmStringable,
              AvmPoint,
              {
                  [AvmStringableToString] = (AvmCallback)AvmPointToString,
              });

AVM_INTERFACES(AvmPoint,
               {
                   interfaceof(AvmStringable, AvmPoint),
               });

AVM_MEMBERS(AvmPoint, AVM_MEMBERS_DEFAULT);

AVM_CLASS_TYPE_EX(AvmPoint, object, AVM_VTABLE_DEFAULT);

AvmString AvmToString(AvmStringable* self)
{
    return AVM_INTERFACE_CALL(AvmStringableToString, AvmString, object)(
        self->Value);
}

void Main()
{
    AvmPoint* p = AvmObjectNew(typeid(AvmPoint));
    p->X = 5.1;
    p->Y = 7.2;
    AvmStringable s = AvmQueryInterface(p, AvmStringable);
    AvmString s1 = AvmToString(&s);

    AvmPrintf("%v\n", &s1);
}
