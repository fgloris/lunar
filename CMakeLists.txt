cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(lunar)

# 添加debug选项，默认为ON
option(DEBUG "Enable debug mode" ON)

if(NOT DEBUG)
    add_compile_definitions(NDEBUG)
else()
    message(STATUS "Debug mode is enabled")
endif()

add_subdirectory(3rdparties/glad)
add_subdirectory(modules/render)

add_executable(${PROJECT_NAME} modules/main/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    modules/render
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    render
)