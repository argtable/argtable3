add_library(argtable3 argtable3.c)
target_include_directories(argtable3 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
    )
install(TARGETS argtable3 EXPORT ${PROJECT_NAME}-config DESTINATION lib)
install(FILES "${CMAKE_CURRENT_LIST_DIR}/argtable3.h" DESTINATION include)
