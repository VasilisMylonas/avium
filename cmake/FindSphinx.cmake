include(FindPackageHandleStandardArgs)

find_program(SPHINX_EXECUTABLE NAMES sphinx-build)
find_package_handle_standard_args(Sphinx SPHINX_EXECUTABLE)
