#include "avium/core.h"

#include "avium/private/thread-context.h"
#include "avium/private/virtual.h"

#include "avium/string.h"
#include "avium/thread.h"
#include "avium/typeinfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <deps/gc.h>

#ifdef AVM_HAVE_BACKTRACE
#include <execinfo.h>
#endif

#define _ AvmRuntimeGetResource

static struct
{
    AvmVersion version;
    str* argv;
    uint argc;
} AvmRuntimeState;

//
// Object functions.
//

object AvmObjectNew(const AvmClass* type)
{
    pre
    {
        assert(type != NULL);
    }

    object o = AvmAlloc(AvmTypeGetSize((const AvmType*)type));
    GC_register_finalizer(o,
                          (GC_finalization_proc)(AvmCallback)AvmObjectFinalize,
                          NULL,
                          NULL,
                          NULL);
    *(const AvmClass**)o = type;

    post
    {
        assert(o != NULL);
    }

    return o;
}

const AvmClass* AvmObjectGetType(object self)
{
    pre
    {
        assert(self != NULL);
    }

    // The first member of an object should be a const AvmClass*
    // Look in core.h for AVM_CLASS
    const AvmClass* type = *(const AvmClass**)self;

    post
    {
        assert(type != NULL);
    }

    return type;
}

void AvmObjectSurpressFinalizer(object self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmRuntimeIsHeapObject(self));
    }

    GC_register_finalizer(self, NULL, NULL, NULL, NULL);
}

void* AvmObjectVisit(object self, const AvmMember* member)
{
    pre
    {
        assert(self != NULL);
        assert(member != NULL);
    }

    return ((byte*)self) + member->_private.offset;
}

//
// Virtual calls.
//

bool AvmObjectEquals(object self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
        assert(AvmObjectGetType(self) == AvmObjectGetType(other));
    }

    VIRTUAL_CALL(bool, FnEntryEquals, self, other);
}

void AvmObjectFinalize(object self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmRuntimeIsHeapObject(self));
    }

    AvmObjectSurpressFinalizer(self);
    VIRTUAL_CALL(void, FnEntryFinalize, self);
}

object AvmObjectClone(object self)
{
    pre
    {
        assert(self != NULL);
    }

    VIRTUAL_CALL(object, FnEntryClone, self);
}

AvmString AvmObjectToString(object self)
{
    pre
    {
        assert(self != NULL);
    }

    VIRTUAL_CALL(AvmString, FnEntryToString, self);
}

//
// AvmLocation type.
//

static AvmString AvmLocationToString(AvmLocation* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(AVM_LOCATION_FMT_STR, self->File, self->Line);
}

AVM_CLASS_TYPE(AvmLocation,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmLocationToString,
               });

AvmLocation AvmLocationFrom(str file, uint line, uint column)
{
    pre
    {
        assert(file != NULL);
    }

    return (AvmLocation){
        .__type = typeid(AvmLocation),
        .File = file,
        .Line = line,
        .Column = column,
    };
}

//
// AvmVersion type.
//

static AvmString AvmVersionToString(AvmVersion* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_VERSION_FMT_STR, self->Major, self->Minor, self->Patch);
}

AVM_CLASS_TYPE(AvmVersion,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmVersionToString,
               });

AvmVersion AvmVersionFrom(ushort major, ushort minor, ushort patch)
{
    return (AvmVersion){
        .__type = typeid(AvmVersion),
        .Major = major,
        .Minor = minor,
        .Patch = patch,
    };
}

//
// AvmArgs type.
//

AVM_CLASS_TYPE(AvmArgs, object, AVM_VTABLE_DEFAULT);

bool AvmArgsNext(AvmArgs* self)
{
    if (self->_private.position == self->Length)
    {
        // Reset
        self->_private.position = 0;
        self->Current = self->_private.argv[0];
        return false;
    }

    self->Current = self->_private.argv[self->_private.position];
    self->_private.position++;
    return true;
}

//
// Builtin runtime functions.
//

AvmExitCode AvmRuntimeInit(int argc, str argv[], AvmEntryPoint entry)
{
    pre
    {
        assert(argc > 0);
        assert(argv != NULL);
        assert(entry != NULL);
    }

    if (AvmRuntimeState.argv != NULL)
    {
        return EXIT_FAILURE;
    }

    // TODO: When does GC start and end?
    GC_INIT();

    AvmRuntimeState.argc = (uint)(argc - 1);
    AvmRuntimeState.argv = argv + 1;
    AvmRuntimeState.version =
        AvmVersionFrom(AVM_VERSION_MAJOR, AVM_VERSION_MINOR, AVM_VERSION_PATCH);

    AvmExitCode code = __AvmRuntimeThreadInit(__AvmThreadContextNew(
        NULL,
        (AvmThreadEntryPoint)entry,
        __AvmThreadNewObject(AVM_THREAD_MAIN_NAME, false, 0)));

    AvmRuntimeState.argv = NULL;

    return code;
}

str AvmRuntimeGetProgramName(void)
{
    return AvmRuntimeState.argv[-1];
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return AvmRuntimeState.version;
}

AvmArgs AvmRuntimeGetArgs(void)
{
    return (AvmArgs){
        .__type = typeid(AvmArgs),
        .Length = AvmRuntimeState.argc,
        ._private =
            {
                .argv = AvmRuntimeState.argv,
                .position = 0,
            },
    };
}

bool AvmRuntimeIsHeapObject(object o)
{
    pre
    {
        assert(o != NULL);
    }

    return GC_is_heap_ptr(o);
}

AvmString AvmRuntimeGetBacktrace(void)
{
#ifdef AVM_HAVE_BACKTRACE
    void* arr[AVM_BACKTRACE_MAX_SYMBOLS];

    int length = backtrace(arr, AVM_BACKTRACE_MAX_SYMBOLS);
    char** s = backtrace_symbols(arr, length);

    AvmString bt = AvmStringNew(length * AVM_BACKTRACE_LINE_SIZE);

    for (int i = length - 1; i >= 1; i--)
    {
        *(strchr(s[i], '+')) = '\0';
        *(strchr(s[i], '(')) = AVM_BACKTRACE_SEPARATOR;

        AvmStringPushStr(&bt, "    in ");
        AvmStringPushStr(&bt, s[i]);
        if (i != 1)
        {
            AvmStringPushChar(&bt, '\n');
        }
    }

    free(s);
    return bt;
#else
    return AvmStringFrom(AVM_BACKTRACE_NOT_AVAILABLE_MSG);
#endif
}

never AvmRuntimeThrow(object value, AvmLocation location)
{
    pre
    {
        assert(value != NULL);
#ifndef AVM_THROW_AUTO_CLONE
        assert(AvmRuntimeIsHeapObject(value));
#endif
    }

#ifdef AVM_THROW_AUTO_CLONE
    if (!AvmRuntimeIsHeapObject(value))
    {
        value = AvmObjectClone(value);
    }
#endif

    const AvmThread* t = AvmThreadGetCurrent();

    t->_context->_location = location;
    t->_context->_thrownObject = value;
    longjmp(t->_context->_jumpBuffer, 1);
}

//
// Allocation functions.
//

void* AvmAlloc(uint size)
{
    pre
    {
        assert(size != 0);
    }

    void* mem = GC_malloc(size);
    if (mem == NULL)
    {
        GC_abort_on_oom();
    }
    return mem;
}

void* AvmRealloc(void* memory, uint size)
{
    pre
    {
        assert(memory != NULL);
        assert(size != 0);
    }

    return GC_realloc(memory, size);
}

//
// Fundamental IO.
//

static void AvmFputs(str format, va_list args, FILE* stream)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
        assert(stream != NULL);
    }

    AvmString temp = AvmStringFormatV(format, args);
    fwrite(AvmStringGetBuffer(&temp), sizeof(char), temp._length, stream);
}

void AvmVScanf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    // TODO: Ideally we should have a custom implementation.
    vscanf(format, args);
}

void AvmVPrintf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stdout);
}

void AvmVErrorf(str format, va_list args)
{
    pre
    {
        assert(format != NULL);
        assert(args != NULL);
    }

    AvmFputs(format, args, stderr);
}

// These just forward to the V- equivalents.

void AvmScanf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVScanf(format, args);
    va_end(args);
}

void AvmPrintf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVPrintf(format, args);
    va_end(args);
}

void AvmErrorf(str format, ...)
{
    pre
    {
        assert(format != NULL);
    }

    va_list args;
    va_start(args, format);
    AvmVErrorf(format, args);
    va_end(args);
}

//
// AvmNativeError
//

AVM_CLASS(AvmNativeError, object, { int _code; });

static AvmString AvmNativeErrorToString(const AvmNativeError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFrom(strerror(self->_code));
}

AVM_CLASS_TYPE(AvmNativeError,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmNativeErrorToString,
               });

AvmError* AvmErrorFromOSCode(int code)
{
    AvmNativeError* e = AvmObjectNew(typeid(AvmNativeError));
    e->_code = code;
    return e;
}

//
// AvmDetailedError
//

AVM_CLASS(AvmDetailedError, object, {
    str _message;
    AvmString _backtrace;
});

static AvmString AvmDetailedErrorToString(const AvmDetailedError* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFormat(
        AVM_DETAILED_ERROR_FMT_STR, self->_message, &self->_backtrace);
}

AVM_CLASS_TYPE(AvmDetailedError,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmDetailedErrorToString,
               });

AvmError* AvmErrorNew(str message)
{
    pre
    {
        assert(message != NULL);
    }

    AvmDetailedError* e = AvmObjectNew(typeid(AvmDetailedError));
    e->_backtrace = AvmRuntimeGetBacktrace();
    e->_message = message;
    return e;
}

//
// Boxing.
//

AvmInteger AvmIntegerFrom(_long value)
{
    return (AvmInteger){
        .__type = typeid(AvmInteger),
        .Value = value,
    };
}

AvmUnsigned AvmUnsignedFrom(ulong value)
{
    return (AvmUnsigned){
        .__type = typeid(AvmUnsigned),
        .Value = value,
    };
}

AvmFloat AvmFloatFrom(double value)
{
    return (AvmFloat){
        .__type = typeid(AvmFloat),
        .Value = value,
    };
}

static AvmString AvmIntegerToString(const AvmInteger* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromInt(self->Value);
}

static bool AvmIntegerEquals(const AvmInteger* self, const AvmInteger* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->Value == other->Value;
}

static AvmString AvmUnsignedToString(const AvmUnsigned* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromUint(self->Value, AvmNumericBaseDecimal);
}

static bool AvmUnsignedEquals(const AvmUnsigned* self, const AvmUnsigned* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->Value == other->Value;
}

static AvmString AvmFloatToString(const AvmFloat* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AvmStringFromFloat(self->Value, AvmFloatReprAuto);
}

static bool AvmFloatEquals(const AvmFloat* self, const AvmFloat* other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
    }

    return self->Value == other->Value;
}

AVM_CLASS_TYPE(AvmInteger,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmIntegerToString,
                   [FnEntryEquals] = (AvmCallback)AvmIntegerEquals,
               });

AVM_CLASS_TYPE(AvmUnsigned,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmUnsignedToString,
                   [FnEntryEquals] = (AvmCallback)AvmUnsignedEquals,
               });

AVM_CLASS_TYPE(AvmFloat,
               object,
               {
                   [FnEntryToString] = (AvmCallback)AvmFloatToString,
                   [FnEntryEquals] = (AvmCallback)AvmFloatEquals,
               });

//
// Exception implementation.
//

AVM_CLASS_TYPE(AvmThrowContext, object, {[FnEntryFinalize] = NULL});

AvmThrowContext* __AvmRuntimeGetThrowContext(void)
{
    return AvmThreadGetCurrent()->_context;
}

void __AvmRuntimePushThrowContext(AvmThrowContext* context)
{
    AvmThread* t = (AvmThread*)AvmThreadGetCurrent();

    context->__type = typeid(AvmThrowContext);
    context->_thrownObject = NULL;
    context->_prev = t->_context;

    lock(&t->_lock)
    {
        t->_context = context;
    }
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThread* t = (AvmThread*)AvmThreadGetCurrent();
    AvmThrowContext* retval = t->_context;

    lock(&t->_lock)
    {
        t->_context = retval->_prev;
    }

    return retval;
}

//
// Misc.
//

void AvmCopy(object o, size_t size, byte* destination)
{
    pre
    {
        assert(o != NULL);
        assert(destination != NULL);
    }

    size_t objectSize = AvmTypeGetSize((const AvmType*)AvmObjectGetType(o));

    if (objectSize > size)
    {
        // TODO: error
    }

    memcpy(destination, o, objectSize);
}

str AvmRuntimeGetResource(AvmResourceKey key)
{
    switch (key)
    {
    case AvmArgErrorMsg:
        return "Received an invalid argument.";
    case AvmMemErrorMsg:
        return "The system run out of memory.";
    case AvmRangeErrorMsg:
        return "The provided index was out of range.";
    case AvmMarshalErrorMsg:
        return "Marshalling of non primitive types is not supported.";
    case AvmMissingMemberErrorMsg:
        return "The requested member was missing.";
    case AvmMissingConstantErrorMsg:
        return "The requested constant was missing.";
    case AvmMissingCallbackErrorMsg:
        return "The requested callback was missing.";
    case AvmThreadCreationErrorMsg:
        return "Thread creation failed.";
    case AvmThreadJoinErrorMsg:
        return "Failed to join thread.";
    case AvmThreadDetachErrorMsg:
        return "Failed to detach thread.";
    case AvmInvalidStackSizeErrorMsg:
        return "Invalid stack size.";
    default:
        return "";
    }
}

#ifdef AVM_INSERT_INIT_CODE
// This should be defined by user code.
extern void AvmMain();

int main(int argc, str argv[])
{
    return AvmRuntimeInit(argc, argv, AvmMain);
}
#endif
