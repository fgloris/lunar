find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB SRC_FILES *.cpp)

set(SUB_LIBRARY_NAME interface)
add_library(${SUB_LIBRARY_NAME} STATIC ${SRC_FILES})

target_include_directories(${SUB_LIBRARY_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/3rdparties/yaml-cpp
)

target_link_libraries(${SUB_LIBRARY_NAME}
    PRIVATE glad yaml-cpp
    PUBLIC glfw
)