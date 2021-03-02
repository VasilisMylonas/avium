add_compile_definitions(AVM_EXPORT)
add_library(avium SHARED core.c)
add_library(avium-static STATIC core.c)
