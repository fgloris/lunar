find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB SRC_FILES *.cpp)

set(SUB_LIBRARY_NAME render)
add_library(${SUB_LIBRARY_NAME} STATIC ${SRC_FILES})

target_include_directories(${SUB_LIBRARY_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/modules
    ${CMAKE_SOURCE_DIR}/3rdparties
)

target_link_libraries(${SUB_LIBRARY_NAME}
    PRIVATE 
        glad
        stb_image
    PUBLIC 
        glfw
        interface
)