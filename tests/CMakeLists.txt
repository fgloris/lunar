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
    ${CMAKE_SOURCE_DIR}/modules
)

include(GoogleTest)
gtest_discover_tests(${TEST_BINARY})

add_executable(${PROJECT_NAME}_smiling_box main-smiling-box.cpp)
target_link_libraries(${PROJECT_NAME}_smiling_box PRIVATE render)

add_executable(${PROJECT_NAME}_phong_shading main-phong-shading.cpp)
target_link_libraries(${PROJECT_NAME}_phong_shading PRIVATE render)