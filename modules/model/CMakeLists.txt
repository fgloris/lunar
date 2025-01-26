find_package(glm CONFIG REQUIRED)

file(GLOB SRC_FILES *.cpp)

set(SUB_LIBRARY_NAME model)
add_library(${SUB_LIBRARY_NAME} STATIC ${SRC_FILES})

target_include_directories(${SUB_LIBRARY_NAME} PUBLIC 
    ${CMAKE_SOURCE_DIR}/3rdparties
    ${CMAKE_SOURCE_DIR}/modules
)

target_link_libraries(${SUB_LIBRARY_NAME}
    PRIVATE glad fmt stb_image
)