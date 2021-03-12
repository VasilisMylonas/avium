set(AVM_SOURCE_FILES core.c string.c options.c fmt.c)

add_library(avm SHARED ${AVM_SOURCE_FILES})
add_library(avm-static STATIC ${AVM_SOURCE_FILES})
add_library(avmtest STATIC testing.c)

if(MSVC)
target_compile_definitions(avm PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(avm-static PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(avmtest PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(avm PRIVATE AVM_EXPORT)
