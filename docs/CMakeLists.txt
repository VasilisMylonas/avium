find_package(Doxygen)
find_package(Sphinx)

if(DOXYGEN_FOUND AND SPHINX_FOUND)
    set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C TRUE)
    set(DOXYGEN_GENERATE_HTML FALSE)
    set(DOXYGEN_GENERATE_LATEX FALSE)
    set(DOXYGEN_GENERATE_XML TRUE)
    set(DOXYGEN_GENERATE_MAN FALSE)
    set(DOXYGEN_TYPEDEF_HIDES_STRUCT TRUE)
    set(DOXYGEN_OUTPUT_DIRECTORY ${DOC_OUTPUT_DIR})
    set(WARN_NO_PARAMDOC TRUE)
    set(HIDE_UNDOC_MEMBERS TRUE)
    set(HIDE_UNDOC_CLASSES TRUE)
    set(DOXYGEN_RECURSIVE TRUE)
    set(DOXYGEN_MACRO_EXPANSION TRUE)
    set(DOXYGEN_EXPAND_ONLY_PREDEF TRUE)
    set(DOXYGEN_PREDEFINED
        DOXYGEN
        AVMAPI=extern
        static_assert=static_assert
        AVM_PTR_TYPE=long
        AVM_LONG_TYPE=long
    )

    configure_file(conf.py.in conf.py @ONLY)

    doxygen_add_docs(Doxygen ${INCLUDE_DIR})
    add_custom_target(docs DEPENDS Doxygen
        COMMAND ${SPHINX_EXECUTABLE} -q -c ${DOC_OUTPUT_DIR}
                ${CMAKE_SOURCE_DIR}/docs ${HTML_OUTPUT_DIR})
else()
    message(WARNING "Install Doxygen and Sphinx to gener ate documentation.")
endif()
