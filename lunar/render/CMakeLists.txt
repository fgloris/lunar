find_package(glm CONFIG REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB SRC_FILES *.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/3rdparties/glad
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    glfw 
    glm::glm
    glad
)