# Moonstone

A 3D game engine written in C++

OpenGL to begin with, but Vulkan will be the primary focus

Built on Linux, for Linux... but I guess it works for Windows too

# Build - Prereqs

`git clone --recurse-submodules --remote-submodules https://github.com/JJB9922/Moonstone`

Assimp requires a bit of custom setup since it builds custom config files per system:
`cd Engine/vendor/`
`git clone https://github.com/assimp/assimp`
`cd assimp`
`cmake .`


# Build - Linux

Clone the repo & submodules using

`git clone --recurse-submodules --remote-submodules https://github.com/JJB9922/Moonstone`

cd into `Moonstone/Engine/`

run:
`chmod +x make-build.sh`
`./make-build.sh`
`cd build`
`./MoonstoneApp`


# Build - Windows*

*please note this is a guess

Clone the repo & submodules using

`git clone --recurse-submodules --remote-submodules https://github.com/JJB9922/Moonstone`

cd into `Moonstone/Engine/`

run:
make a folder called "build"
cd into the `build` folder
run `cmake ..`
run `cmake --build .`
run the resulting MoonstoneApp executable.

Good luck working out the rest, I didn't want to

