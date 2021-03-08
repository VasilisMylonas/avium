set(AVM_SOURCE_FILES core.c string.c options.c)

add_library(avium SHARED ${AVM_SOURCE_FILES})
add_library(avium-static STATIC ${AVM_SOURCE_FILES})
add_library(avmtest STATIC testing.c)

if(MSVC)
target_compile_definitions(avium PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(avium-static PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(avmtest PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(avium PRIVATE AVM_EXPORT)
