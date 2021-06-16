#include "avium/core.h"

#include "avium/private/task-context.h"
#include "avium/private/thread-context.h"

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

    object* o = AvmAlloc(
        AvmTypeGetSize((const AvmType*)type) + sizeof(AvmMutex*), true);
    GC_register_finalizer(o,
                          (GC_finalization_proc)(AvmCallback)AvmObjectFinalize,
                          NULL,
                          NULL,
                          NULL);

    o[0] = NULL;         // No mutex yet.
    o[1] = (object)type; // Type.

    post
    {
        assert(o != NULL);
    }

    return &o[1];
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

    void* ret = ((byte*)self) + baseof(member)->_private.offset;

    post
    {
        assert(ret != NULL);
    }

    return ret;
}

void AvmObjectLock(object self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmRuntimeIsHeapObject(self));
    }

    AvmMutex** o = self;

    if (o[-1] == NULL)
    {
        // Lazy initialization.
        AvmMutex* mutex = AvmObjectNew(typeid(AvmMutex));
        *mutex = AvmMutexNew(false);
        o[-1] = mutex;
    }

    AvmMutexLock(o[-1]);
}

void AvmObjectUnlock(object self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmRuntimeIsHeapObject(self));
    }

    AvmMutex** o = self;
    AvmMutexUnlock(o[-1]);
}

//
// Virtual calls.
//

void AvmObjectFinalize(object self)
{
    pre
    {
        assert(self != NULL);
        assert(AvmRuntimeIsHeapObject(self));
    }

    AvmObjectSurpressFinalizer(self);
    AVM_VCALL(AvmEntryFinalize, void, object)(self);
}

AvmString AvmObjectToString(object self)
{
    pre
    {
        assert(self != NULL);
    }

    return AVM_VCALL(AvmEntryToString, AvmString, object)(self);
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
                   [AvmEntryToString] = (AvmCallback)AvmLocationToString,
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
                   [AvmEntryToString] = (AvmCallback)AvmVersionToString,
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
    pre
    {
        assert(self != NULL);
    }

    self->_private.position++;

    if (self->_private.position == self->_private.length)
    {
        // Reset
        self->_private.position = 0;
        return false;
    }

    return true;
}

str AvmArgsGetCurrent(const AvmArgs* self)
{
    pre
    {
        assert(self != NULL);
    }

    str ret = self->_private.argv[self->_private.position];

    post
    {
        assert(ret != NULL);
    }

    return ret;
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

    // Initialize task subsystem.
    __AvmRuntimeTaskInit();

    AvmExitCode code = __AvmRuntimeThreadInit(__AvmThreadContextNew(
        NULL,
        (AvmThreadEntryPoint)entry,
        __AvmThreadNewObject(AVM_THREAD_MAIN_NAME, false, 0)));

    AvmRuntimeState.argv = NULL;

    return code;
}

str AvmRuntimeGetProgramName(void)
{
    str ret = AvmRuntimeState.argv[-1];

    post
    {
        assert(ret != NULL);
    }

    return ret;
}

AvmVersion AvmRuntimeGetVersion(void)
{
    return AvmRuntimeState.version;
}

AvmArgs AvmRuntimeGetArgs(void)
{
    return (AvmArgs){
        .__type = typeid(AvmArgs),
        ._private =
            {
                .length = AvmRuntimeState.argc,
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
        AvmCloneable c = AvmQueryInterface(value, AvmCloneable);
        value = AvmClone(&c);
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

void* AvmAlloc(uint size, bool containsPointers)
{
    pre
    {
        assert(size != 0);
    }

    void* mem = containsPointers ? GC_malloc(size) : GC_malloc_atomic(size);

    if (!containsPointers)
    {
        // GC_malloc_atomic does not clear memory.
        bzero(mem, size);
    }

    if (mem == NULL)
    {
        GC_abort_on_oom();
    }

    post
    {
        assert(mem != NULL);
    }

    return mem;
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
                   [AvmEntryToString] = (AvmCallback)AvmNativeErrorToString,
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
                   [AvmEntryToString] = (AvmCallback)AvmDetailedErrorToString,
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

AVM_IMPLEMENT(AvmEquatable,
              AvmInteger,
              {
                  [AvmEquatableEquals] = (AvmCallback)AvmIntegerEquals,
              });

AVM_IMPLEMENT(AvmEquatable,
              AvmUnsigned,
              {
                  [AvmEquatableEquals] = (AvmCallback)AvmUnsignedEquals,
              });

AVM_IMPLEMENT(AvmEquatable,
              AvmFloat,
              {
                  [AvmEquatableEquals] = (AvmCallback)AvmFloatEquals,
              });

AVM_INTERFACES(AvmInteger,
               {
                   interfaceid(AvmEquatable, AvmInteger),
               });

AVM_INTERFACES(AvmUnsigned,
               {
                   interfaceid(AvmEquatable, AvmUnsigned),
               });

AVM_INTERFACES(AvmFloat,
               {
                   interfaceid(AvmEquatable, AvmFloat),
               });

AVM_MEMBERS(AvmInteger, AVM_MEMBERS_DEFAULT);
AVM_MEMBERS(AvmUnsigned, AVM_MEMBERS_DEFAULT);
AVM_MEMBERS(AvmFloat, AVM_MEMBERS_DEFAULT);

AVM_CLASS_TYPE_EX(AvmInteger,
                  object,
                  {
                      [AvmEntryToString] = (AvmCallback)AvmIntegerToString,
                  });

AVM_CLASS_TYPE_EX(AvmUnsigned,
                  object,
                  {
                      [AvmEntryToString] = (AvmCallback)AvmUnsignedToString,
                  });

AVM_CLASS_TYPE_EX(AvmFloat,
                  object,
                  {
                      [AvmEntryToString] = (AvmCallback)AvmFloatToString,
                  });

//
// Exception implementation.
//

AVM_CLASS_TYPE(AvmThrowContext, object, AVM_VTABLE_DEFAULT);

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

    AvmMutexLock(&t->_lock);
    t->_context = context;
    AvmMutexUnlock(&t->_lock);
}

AvmThrowContext* __AvmRuntimePopThrowContext(void)
{
    AvmThread* t = (AvmThread*)AvmThreadGetCurrent();
    AvmThrowContext* retval = t->_context;

    AvmMutexLock(&t->_lock);
    t->_context = retval->_prev;
    AvmMutexUnlock(&t->_lock);

    return retval;
}

//
// Misc.
//

bool AvmEquals(const AvmEquatable* self, object other)
{
    pre
    {
        assert(self != NULL);
        assert(other != NULL);
        assert(AvmObjectGetType(self->Value) == AvmObjectGetType(other));
    }

    return AVM_ICALL(AvmEquatableEquals, bool, object, object)(self->Value,
                                                               other);
}

object AvmClone(const AvmCloneable* self)
{
    pre
    {
        assert(self != NULL);
    }

    return AVM_ICALL(AvmCloneableClone, object, object)(self->Value);
}

AVM_CLASS_TYPE(AvmEquatable, object, AVM_VTABLE_DEFAULT);
AVM_CLASS_TYPE(AvmCloneable, object, AVM_VTABLE_DEFAULT);

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
    case AvmMissingInterfaceErrorMsg:
        return "The requested interface was missing.";
    case AvmTaskReturnErrorMsg:
        return "Cannot return from the main task.";
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
