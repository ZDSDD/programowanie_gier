cmake_minimum_required(VERSION 3.28)
project(projekt1)

# Define variables
set(GLFW_INCLUDE_DIR "F:/Libs/glfw-3.4.bin.WIN64/include")
set(GLAD_INCLUDE_DIR "F:/Libs/glad/include")
set(GLM_INCLUDE_DIR "F:/Libs/glm-1.0.1-light")
set(GLAD_SOURCE_DIR "F:/Libs/glad/src")
set(GLFW_LIB_DIR "F:/Libs/glfw-3.4.bin.WIN64/lib-mingw-w64")

# Add GLFW library
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_LIB_DIR}/libglfw3.a")
# Add GLAD library
add_library(glad ${GLAD_SOURCE_DIR}/glad.c)
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})
set(CMAKE_CXX_STANDARD 17)

add_executable(projekt1 rotating.cpp ShaderUtils.cpp)

target_include_directories(projekt1 PUBLIC ${GLFW_INCLUDE_DIR})
# Set include directories after add_executable
target_include_directories(projekt1
        PUBLIC ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
)

# Link GLFW and GLAD libraries to your executable
target_link_libraries(projekt1 glfw glad)
# Additional Dependencies
target_link_libraries(projekt1 opengl32 user32 gdi32 shell32)