#ifndef AVIUM_ALIASES_H
#define AVIUM_ALIASES_H

// types.h
#define CLASS     AVM_CLASS
#define TYPE      AVM_TYPE
#define INTERFACE AVM_INTERFACE

#define FORWARD_TYPE AVM_FORWARD_TYPE
#define CONCAT       CONCAT
#define UNIQUE       AVM_UNIQUE

#define Function AvmFunction
#define Type     AvmType
#define Error    AvmError
#define String   AvmString

// error.h
#define ErrorKind         AvmErrorKind
#define ErrorGetLast      AvmErrorGetLast
#define ErrorFromOSCode   AvmErrorFromOSCode
#define ErrorOfKind       AvmErrorOfKind
#define ErrorGetSource    AvmErrorGetSource
#define ErrorGetBacktrace AvmErrorGetBacktrace
#define Panic             AvmPanic
#define PanicEx           AvmPanicEx

// core.h
#define Version                  AvmVersion
#define ObjectGetType            AvmObjectGetType
#define ObjectEquals             AvmObjectEquals
#define ObjectDestroy            AvmObjectDestroy
#define ObjectClone              AvmObjectClone
#define ObjectToString           AvmObjectToString
#define VersionFrom              AvmVersionFrom
#define RuntimeGetVersion        AvmRuntimeGetVersion
#define RuntimeInit              AvmRuntimeInit
#define RuntimeGetProgramName    AvmRuntimeGetProgramName
#define RuntimeEnableExceptions  AvmRuntimeEnableExceptions
#define RuntimeDisableExceptions AvmRuntimeDisableExceptions
#define Alloc                    AvmAlloc
#define Realloc                  AvmRealloc
#define Dealloc                  AvmDealloc
#define Copy                     AvmCopy
#define VScanf                   AvmVScanf
#define VPrintf                  AvmVPrintf
#define VErrorf                  AvmVErrorf
#define Scanf                    AvmScanf
#define Printf                   AvmPrintf
#define Errorf                   AvmErrorf

// typeinfo.h
#define FnEntry          AvmFnEntry
#define TypeGetName      AvmTypeGetName
#define TypeGetSize      AvmTypeGetSize
#define TypeGetFunction  AvmTypeGetFunction
#define TypeGetBase      AvmTypeGetBase
#define TypeInheritsFrom AvmTypeInheritsFrom

// TODO

// string.h
#define StringNew              AvmStringNew
#define StringFrom             AvmStringFrom
#define StringFromChars        AvmStringFromChars
#define StringRepeat           AvmStringRepeat
#define StringRepeatChars      AvmStringRepeatChars
#define StringGetLength        AvmStringGetLength
#define StringGetCapacity      AvmStringGetCapacity
#define StringPushChar         AvmStringPushChar
#define StringPushStr          AvmStringPushStr
#define StringPushChars        AvmStringPushChars
#define StringPushString       AvmStringPushString
#define StringIndexOf          AvmStringIndexOf
#define StringLastIndexOf      AvmStringLastIndexOf
#define StringReplace          AvmStringReplace
#define StringReplaceN         AvmStringReplaceN
#define StringReplaceLastN     AvmStringReplaceLastN
#define StringReplaceLast      AvmStringReplaceLast
#define StringReplaceAll       AvmStringReplaceAll
#define StringFind             AvmStringFind
#define StringFindLast         AvmStringFindLast
#define StringCharAt           AvmStringCharAt
#define StringReverse          AvmStringReverse
#define StringToUpper          AvmStringToUpper
#define StringToLower          AvmStringToLower
#define StringUnsafeSetLength  AvmStringUnsafeSetLength
#define StringGetBuffer        AvmStringGetBuffer
#define StringUnsafeFromRaw    AvmStringUnsafeFromRaw
#define StringUnsafeDestruct   AvmStringUnsafeDestruct
#define StringIsEmpty          AvmStringIsEmpty
#define StringClear            AvmStringClear
#define StringErase            AvmStringErase
#define StringContainsChar     AvmStringContainsChar
#define StringContainsStr      AvmStringContainsStr
#define StringStartsWithChar   AvmStringStartsWithChar
#define StringStartsWithChars  AvmStringStartsWithChars
#define StringStartsWithStr    AvmStringStartsWithStr
#define StringStartsWithString AvmStringStartsWithString
#define StringEndsWithChar     AvmStringEndsWithChar
#define StringEndsWithStr      AvmStringEndsWithStr
#define StringEndsWithChars    AvmStringEndsWithChars
#define StringEndsWithString   AvmStringEndsWithString
#define StringContainsChars    AvmStringContainsChars
#define StringContainsString   AvmStringContainsString

// path.h
#define PathGetSeparator    AvmPathGetSeparator
#define PathGetAltSeparator AvmPathGetAltSeparator
#define PathHasExtension    AvmPathHasExtension
#define PathIsAbsolute      AvmPathIsAbsolute
#define PathIsRelative      AvmPathIsRelative
#define PathIsValid         AvmPathIsValid
#define PathIsDir           AvmPathIsDir
#define PathGetName         AvmPathGetName
#define PathGetExtension    AvmPathGetExtension
#define PathGetParent       AvmPathGetParent
#define PathGetHomeDir      AvmPathGetHomeDir
#define PathGetTempDir      AvmPathGetTempDir
#define PathGetFullPath     AvmPathGetFullPath
#define PathCombine         AvmPathCombine
#define PathCombineV        AvmPathCombineV
#define PathCombine2        AvmPathCombine2

#endif // AVIUM_ALIASES_H
