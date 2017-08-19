if (NOT WIN32)
    add_custom_target(examples
		COMMAND make
		WORKING_DIRECTORY ${argtable3_SOURCE_DIR}/examples)
else()
    add_custom_target(examples
		COMMAND nmake /f Makefile.nmake
		WORKING_DIRECTORY ${argtable3_SOURCE_DIR}/examples)
endif()
