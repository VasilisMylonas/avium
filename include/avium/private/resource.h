#ifndef AVIUM_PRIVATE_RESOURCE_H
#define AVIUM_PRIVATE_RESOURCE_H

typedef enum
{
    AvmArgErrorMsg,
    AvmMemErrorMsg,
    AvmRangeErrorMsg,
    AvmMarshalErrorMsg,
    AvmMissingMemberErrorMsg,
    AvmMissingConstantErrorMsg,
    AvmMissingCallbackErrorMsg,
    AvmThreadCreationErrorMsg,
    AvmThreadJoinErrorMsg,
    AvmThreadDetachErrorMsg,
    AvmInvalidStackSizeErrorMsg,
    AvmMissingInterfaceErrorMsg,
} AvmResourceKey;

extern str __AvmRuntimeGetResource(AvmResourceKey key);

#define _ __AvmRuntimeGetResource

#endif // AVIUM_PRIVATE_RESOURCE_H
