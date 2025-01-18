cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)

project(lunar)

# debug选项默认为OFF, 为了编译速度
option(DEBUG "Enable debug mode" OFF)
option(REFETCH "Refetch all 3rd parties" OFF)


include(FetchContent)
set(FETCHCONTENT_QUIET OFF)

if (REFETCH OR NOT EXISTS ${CMAKE_SOURCE_DIR}/3rdparties/fmt)
    FetchContent_Declare(
    fmt
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparties/fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281 # 10.2.1
    GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(fmt)
else()
    add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparties/fmt)
endif()

add_subdirectory(3rdparties/glad)
add_subdirectory(modules/render)

if(NOT DEBUG OR NDEBUG)
    add_compile_definitions(NDEBUG)
else()
    message(WARNING "Debug mode is enabled, enabling googletest")
    if (REFETCH OR NOT EXISTS ${CMAKE_SOURCE_DIR}/3rdparties/googletest)
        FetchContent_Declare(
            googletest
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/3rdparties/googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        e4ece4881d1fefc1e67d21c7493835815cd13085 # 1.15
            GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(googletest)
    else()
        add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparties/googletest)
    endif()
    add_subdirectory(tests)
endif()

add_executable(${PROJECT_NAME} modules/main/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    modules/render
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    render
)