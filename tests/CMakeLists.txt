enable_testing()
set(TEST_BINARY ${PROJECT_NAME}_test)

add_executable(${TEST_BINARY}
    test_open_window.cpp
    test_glm.cpp
)

target_link_libraries(${TEST_BINARY}
    PRIVATE
    render
    GTest::gtest
    GTest::gtest_main
)

target_include_directories(${TEST_BINARY}
    PRIVATE
    ${CMAKE_SOURCE_DIR}/modules/render
)

include(GoogleTest)
gtest_discover_tests(${TEST_BINARY})