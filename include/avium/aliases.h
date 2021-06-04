#ifndef AVIUM_ALIASES_H
#define AVIUM_ALIASES_H

// core.h
#define Callback                AvmCallback
#define Type                    AvmType
#define Enum                    AvmEnum
#define String                  AvmString
#define CLASS                   AVM_CLASS
#define INTERFACE               AVM_INTERFACE
#define ENUM                    AVM_ENUM
#define CONCAT                  CONCAT
#define UNIQUE                  AVM_UNIQUE
#define VA_ARGS                 AVM_VA_ARGS
#define ObjectNew               AvmObjectNew
#define ObjectGetType           AvmObjectGetType
#define ObjectEquals            AvmObjectEquals
#define ObjectFinalize          AvmObjectFinalize
#define ObjectSurpressFinalizer AvmObjectSurpressFinalizer
#define ObjectClone             AvmObjectClone
#define ObjectToString          AvmObjectToString
#define Location                AvmLocation
#define LocationFrom            AvmLocationFrom
#define Version                 AvmVersion
#define VersionFrom             AvmVersionFrom
#define ThrowContext            AvmThrowContext
#define Runtime                 AvmRuntime
#define EntryPoint              AvmEntryPoint
#define RuntimeInit             AvmRuntimeInit
#define RuntimeGetProgramName   AvmRuntimeGetProgramName
#define RuntimeGetVersion       AvmRuntimeGetVersion
#define RuntimeGetArgCount      AvmRuntimeGetArgCount
#define RuntimeGetArgs          AvmRuntimeGetArgs
#define RuntimeGetBacktrace     AvmRuntimeGetBacktrace
#define RuntimeThrow            AvmRuntimeThrow
#define Alloc                   AvmAlloc
#define Realloc                 AvmRealloc
#define VScanf                  AvmVScanf
#define VPrintf                 AvmVPrintf
#define VErrorf                 AvmVErrorf
#define Scanf                   AvmScanf
#define Printf                  AvmPrintf
#define Errorf                  AvmErrorf
#define Error                   AvmError
#define ErrorNew                AvmErrorNew
#define ErrorFromOSCode         AvmErrorFromOSCode
#define Copy                    AvmCopy

// typeinfo.h
#define TYPE             AVM_TYPE
#define ENUM_MEMBER      AVM_ENUM_MEMBER
#define ENUM_TYPE        AVM_ENUM_TYPE
#define FnEntry          AvmFnEntry
#define TypeGetName      AvmTypeGetName
#define TypeGetSize      AvmTypeGetSize
#define TypeGetFunction  AvmTypeGetFunction
#define TypeGetBase      AvmTypeGetBase
#define TypeInheritsFrom AvmTypeInheritsFrom
#define TypeConstruct    AvmTypeConstruct
#define EnumGetName      AvmEnumGetName
#define EnumGetSize      AvmEnumGetSize
#define EnumIsDefined    AvmEnumIsDefined
#define EnumGetNameOf    AvmEnumGetNameOf
#define EnumGetValueOf   AvmEnumGetValueOf

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
