cmake_minimum_required(VERSION 3.16)
project(lunar)

# 添加debug选项，默认为ON
option(DEBUG "Enable debug mode" ON)

set(CMAKE_CXX_STANDARD 20)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

file(GLOB SRC_FILES src/*.cpp glad/glad.c)

add_executable(${PROJECT_NAME} ${SRC_FILES})

# 根据DEBUG选项设置编译定义
if(NOT DEBUG)
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG)
else()
    message(STATUS "Debug mode is enabled")
endif()

target_include_directories(${PROJECT_NAME} ${OPENGL_INCLUDE_DIRS} PRIVATE glad/ include/)
target_link_libraries(${PROJECT_NAME} 
    glfw 
    glm::glm 
    OpenGL::OpenGL
)
