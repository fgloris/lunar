find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB SRC_FILES *.cpp)

add_library(render STATIC ${SRC_FILES})

target_include_directories(render PUBLIC 
    ${CMAKE_SOURCE_DIR}/3rdparties/glad
)

target_link_libraries(render
    PRIVATE glad
    PUBLIC glfw 
)