if (NOT WIN32)
    add_custom_target(tests
		COMMAND make
		WORKING_DIRECTORY ${argtable3_SOURCE_DIR}/tests)
else()
    add_custom_target(tests
		COMMAND nmake /f Makefile.nmake
		WORKING_DIRECTORY ${argtable3_SOURCE_DIR}/tests)
endif()
