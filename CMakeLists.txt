cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(lunar)

# debug选项默认为OFF, 为了编译速度
option(DEBUG "Enable debug mode" OFF)

include(FetchContent)

FetchContent_Declare(
  fmt
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparties/fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

add_subdirectory(3rdparties/glad)
add_subdirectory(modules/render)

if(NOT DEBUG OR NDEBUG)
    add_compile_definitions(NDEBUG)
else()
    message(WARNING "Debug mode is enabled, enabling googletest")
    FetchContent_Declare(
        googletest
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparties/googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG        e4ece4881d1fefc1e67d21c7493835815cd13085) # 1.15
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()


add_executable(${PROJECT_NAME} modules/main/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    modules/render
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    render
)