cmake_minimum_required(VERSION 3.28)
project(projekt1)

# Define paths to external libraries
set(GLFW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glfw-3.4.bin.WIN64/include")
set(GLFW_LIB_DIR "${PROJECT_SOURCE_DIR}/Libs/glfw-3.4.bin.WIN64/lib-mingw-w64")
set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glad/include")
set(GLAD_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Libs/glad/src")
set(GLM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glm-1.0.1-light")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add GLFW library
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_LIB_DIR}/libglfw3.a")

# Add GLAD library
add_library(glad ${GLAD_SOURCE_DIR}/glad.c)
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

# Add executable and link libraries
add_executable(projekt1 sources/cottage2.cpp sources/ShaderUtils.cpp headers/ShaderUtils.hpp
        sources/Shader_s.cpp
        headers/Shader_s.h)
target_include_directories(projekt1 PUBLIC
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
)
target_link_libraries(projekt1 glfw glad opengl32 user32 gdi32 shell32)

# Copy shaders to build directory
file(COPY shaders DESTINATION "${CMAKE_BINARY_DIR}")
