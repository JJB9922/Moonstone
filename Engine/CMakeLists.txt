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
set(SRC_TOOLS_DIR ${CMAKE_SOURCE_DIR}/src/Tools/)

# Glob source files
file(GLOB_RECURSE SRC_CORE_FILES
    "${SRC_CORE_DIR}/*.cpp"
    "${SRC_CORE_DIR}/Include/*.h"

    "${SRC_CORE_DIR}/Events/*.cpp"
    "${SRC_CORE_DIR}/Events/Include/*.h"

    "${SRC_TOOLS_DIR}/*/*.cpp"
    "${SRC_TOOLS_DIR}/*/Include/*h"
)

file(GLOB GLOBAL_FILES
    "${GLOBALS_DIR}/*.h"
)

# Add vendor subdirectories
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/spdlog)

# Create the Moonstone library
add_library(Moonstone STATIC ${SRC_CORE_FILES} ${GLOBAL_FILES})

# Include directories for the library
target_include_directories(Moonstone PUBLIC
    ${GLOBALS_DIR}
    ${SRC_CORE_DIR}
    ${SRC_CORE_EVENTS_DIR}
    ${SRC_TOOLS_DIR}
    vendor/glfw
    vendor/spdlog
)

# Link libraries to Moonstone
target_link_libraries(Moonstone PUBLIC glfw)
target_link_libraries(Moonstone PRIVATE spdlog::spdlog)

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
