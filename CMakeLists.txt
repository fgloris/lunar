cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(lunar)

# debug选项默认为OFF, 为了编译速度
option(DEBUG "Enable debug mode" OFF)

add_subdirectory(3rdparties/glad)
add_subdirectory(modules/render)

if(NOT DEBUG OR NDEBUG)
    add_compile_definitions(NDEBUG)
else()
    message(WARNING "Debug mode is enabled, enabling googletest")
    add_subdirectory(3rdparties/googletest)
    add_subdirectory(tests)
endif()


add_executable(${PROJECT_NAME} modules/main/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    modules/render
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    render
)