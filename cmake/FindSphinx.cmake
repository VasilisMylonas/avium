include(FindPackageHandleStandardArgs)

find_program(SPHINX_EXECUTABLE NAMES sphinx-build)
find_package_handle_standard_args(SPHINX SPHINX_EXECUTABLE)