# Initialize project
cmake_minimum_required(VERSION 3.16...3.25)

project(Moonstone
  VERSION 1.0
  DESCRIPTION "The Moonstone Game Engine"
  LANGUAGES CXX
)

# Set C++17 as global standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Prepare source files
set(GLOBALS_DIR ${CMAKE_SOURCE_DIR}/src/)
set(SRC_CORE_DIR ${CMAKE_SOURCE_DIR}/src/Core/)
set(SRC_RENDERER_DIR ${CMAKE_SOURCE_DIR}/src/Renderer/)
set(SRC_TOOLS_DIR ${CMAKE_SOURCE_DIR}/src/Tools/)
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/src/Shaders/)

# Glob source files
file(GLOB_RECURSE SRC_CORE_FILES
    "${SRC_CORE_DIR}/*.cpp"
    "${SRC_CORE_DIR}/Include/*.h"

    "${SRC_CORE_DIR}/Events/*.cpp"
    "${SRC_CORE_DIR}/Events/Include/*.h"

    "${SRC_CORE_DIR}/Layers/*.cpp"
    "${SRC_CORE_DIR}/Layers/Include/*.h"

    "${SRC_TOOLS_DIR}/*.cpp"
    "${SRC_TOOLS_DIR}/Include/*.h"

    "${SRC_TOOLS_DIR}/*/*.cpp"
    "${SRC_TOOLS_DIR}/*/Include/*h"

    "${SRC_RENDERER_DIR}/*.cpp"
    "${SRC_RENDERER_DIR}/Include/*h"

    "${SRC_RENDERER_DIR}/*/*.cpp"
    "${SRC_RENDERER_DIR}/*/Include/*h"

    "${SRC_RENDERER_DIR}/Platform/*/*.cpp"
    "${SRC_RENDERER_DIR}/Platform/*/Include/*h"

    "${SHADER_DIR}/*.cpp"
    "${SHADER_DIR}/Include/*.h"


)

file(GLOB GLOBAL_FILES
    "${GLOBALS_DIR}/*.h"
)

# Add vendor subdirectories
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

# Static libraries

# ImGui
set (IMGUI_PATH "vendor/imgui")
include_directories(${CMAKE_SOURCE_DIR}/vendor/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/*.cpp a
    ${IMGUI_PATH}/*.h

    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_vulkan.h
    ${IMGUI_PATH}/backends/imgui_impl_vulkan.cpp
)


add_library("ImGui" STATIC ${IMGUI_SOURCES})
target_include_directories("ImGui" PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends)

# STB
include_directories(${CMAKE_SOURCE_DIR}/vendor/stb)
link_directories(${CMAKE_SOURCE_DIR}/vendor/stb)

# Create the Moonstone library
add_library(Moonstone STATIC ${SRC_CORE_FILES} ${GLOBAL_FILES})

# Include directories for the library
target_include_directories(Moonstone PUBLIC
    ${GLOBALS_DIR}
    ${SRC_CORE_DIR}
    ${SRC_RENDERER_DIR}
    ${SRC_TOOLS_DIR}
    ${SHADERS_DIR}
    vendor/glfw
    vendor/spdlog/include
    vendor/glad/include
    vendor/glm/glm
    vendor/imgui
)

# Link libraries to Moonstone
target_link_libraries(Moonstone PRIVATE glfw ImGui spdlog::spdlog glad glm)

# Define the executable
add_executable(MoonstoneApp "${SRC_CORE_DIR}/EntryPoint.cpp")

# Link the library to the executable
target_link_libraries(MoonstoneApp PRIVATE Moonstone)

# Include directories for the executable
target_include_directories(MoonstoneApp PRIVATE
    ${GLOBALS_DIR}
    ${SRC_CORE_DIR}
)

# Set up PCHs for the executable
target_precompile_headers(MoonstoneApp PRIVATE ${CMAKE_SOURCE_DIR}/src/mspch.h)

# Custom Dirs
add_definitions(-DRESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
