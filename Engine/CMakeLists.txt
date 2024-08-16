cmake_minimum_required(VERSION 3.15...3.25)

project(Moonstone
  VERSION
    1.0
  DESCRIPTION
    "The Moonstone Game Engine"
  LANGUAGES
    CXX
)

set_property(TARGET Moonstone PROPERTY CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src/")

add_executable(Moonstone ${SRC_FILES})
