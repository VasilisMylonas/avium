add_compile_definitions(AVM_EXPORT)
add_library(avium SHARED core.c string.c options.c)
add_library(avium-static STATIC core.c string.c options.c)
add_library(avmtest STATIC testing.c)
