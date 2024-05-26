cmake_minimum_required(VERSION 3.27)
project(projekt1)

#Path roots
set(IRRKLANG_ROOT "${PROJECT_SOURCE_DIR}/Libs/irrKlang-64bit-1.6.0")

# Define paths to external libraries
set(GLFW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glfw-3.4.bin.WIN64/include")
set(GLFW_LIB_DIR "${PROJECT_SOURCE_DIR}/Libs/glfw-3.4.bin.WIN64/lib-vc2022")
set(GLAD_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glad/include")
set(GLAD_SOURCE_DIR "${PROJECT_SOURCE_DIR}/Libs/glad/src")
set(GLM_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Libs/glm-1.0.1-light")
set(IRRKLANG_INCLUDE_DIR "${IRRKLANG_ROOT}/include")
set(IRRKLANG_LIB_DIR "${IRRKLANG_ROOT}/lib/Winx64-visualStudio")
set(IRRKLANG_BIN_DIR "${IRRKLANG_ROOT}/bin/Winx64-visualStudio")
# fonts
set(FREETYPE_ROOT "${PROJECT_SOURCE_DIR}/Libs/freetype-windows-binaries-2.13.2")
set(FREETYPE_INCLUDE_DIR "${FREETYPE_ROOT}/include")
set(FREETYPE_LIB_DIR "${FREETYPE_ROOT}/release static/vs2015-2022/win64")
set(FREETYPE_LIBRARY "${FREETYPE_LIB_DIR}/freetype.lib")
find_package(Freetype REQUIRED)
if (EXISTS "${FREETYPE_LIBRARY}")
    message(STATUS "Found FreeType library: ${FREETYPE_LIBRARY}")
else()
    message(FATAL_ERROR "FreeType library not found at ${FREETYPE_LIBRARY}")
endif()
# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add GLFW library
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3.lib")

# Add IrrKlang library
include_directories(${IRRKLANG_INCLUDE_DIR})
set(IRRKLANG_LIBRARY "${IRRKLANG_LIB_DIR}/irrKlang.lib")

if (EXISTS "${IRRKLANG_LIBRARY}")
    message(STATUS "Found irrKlang library: ${IRRKLANG_LIBRARY}")
else()
    message(FATAL_ERROR "irrKlang library not found at ${IRRKLANG_LIBRARY}")
endif()

# Add GLAD library
add_library(glad ${GLAD_SOURCE_DIR}/glad.c)
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

# Add executable and link libraries
add_executable(projekt1
        Game/Game.cpp
        Game/Game.h
        Game/Shader.cpp
        Game/Shader.h
        Game/Texture2D.cpp
        Game/Texture2D.h
        Game/ResourceManager.cpp
        Game/ResourceManager.h
        Game/Main.cpp
        Game/SpriteRenderer.cpp
        Game/SpriteRenderer.h
        Game/GameLevel.cpp
        Game/GameLevel.h
        Game/GameObject.cpp
        Game/GameObject.h
        Game/BallObject.cpp
        Game/BallObject.h
        Game/ParticleGenerator.cpp
        Game/ParticleGenerator.h
        Game/PostProcessor.cpp
        Game/PostProcessor.h
        Game/PowerUp.h
        Game/TextRenderer.cpp
        Game/TextRenderer.h
)

target_include_directories(projekt1 PUBLIC
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${GLM_INCLUDE_DIR}
        ${IRRKLANG_INCLUDE_DIR}
        ${FREETYPE_INCLUDE_DIR}
)
target_link_libraries(projekt1 ${IRRKLANG_LIBRARY} ${FREETYPE_LIBRARY} glfw glad opengl32 user32 gdi32 shell32)

# Copy shaders to build directory
add_custom_target(copy_resources ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/resources
        ${PROJECT_BINARY_DIR}/resources
        COMMENT "Copying resources into binary directory")

add_dependencies(projekt1 copy_resources)
