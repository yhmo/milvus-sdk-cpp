find_package(GTest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/include)

if (${GTest_FOUND})
    message(STATUS "Using gtest from system")
    include_directories(${GTEST_INCLUDE_DIRS})
else ()
    message(STATUS "Using gtest from extranal project")
    build_gtest()
    set(GTEST_LIBRARIES gtest)
    set(GTEST_MAIN_LIBRARIES gtest_main)
endif ()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/ut ut_files)
add_executable(testing-ut ${ut_files})
target_link_libraries(testing-ut PRIVATE milvus_sdk ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} pthread)
gtest_discover_tests(testing-ut)
