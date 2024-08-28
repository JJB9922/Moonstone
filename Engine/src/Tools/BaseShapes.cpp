#include "Tools/Include/BaseShapes.h"

namespace Moonstone
{

namespace Tools
{
float BaseShapes::gridVertices[] = {
    -100.0f, 0.0f, -100.0f, 100.0f,  0.0f, -100.0f,

    -100.0f, 0.0f, 100.0f,  100.0f,  0.0f, 100.0f,

    -100.0f, 0.0f, -100.0f, -100.0f, 0.0f, 100.0f,

    100.0f,  0.0f, -100.0f, 100.0f,  0.0f, 100.0f,
};

const size_t BaseShapes::gridVerticesSize = sizeof(gridVertices);

float BaseShapes::cubeVertices[] = {-1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f,
                                    1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
                                    -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f,

                                    -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
                                    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
                                    -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,

                                    -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  0.0f,
                                    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,  0.0f,
                                    -1.0f, -1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 0.0f,  0.0f,

                                    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  0.0f,
                                    1.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  0.0f,  0.0f,
                                    1.0f,  -1.0f, 1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,

                                    -1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,  1.0f,  -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,
                                    1.0f,  -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  1.0f,  -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,
                                    -1.0f, -1.0f, 1.0f,  0.0f,  -1.0f, 0.0f,  -1.0f, -1.0f, -1.0f, 0.0f,  -1.0f, 0.0f,

                                    -1.0f, 1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  -1.0f, 0.0f,  1.0f,  0.0f,
                                    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
                                    -1.0f, 1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  -1.0f, 1.0f,  -1.0f, 0.0f,  1.0f,  0.0f}

;

const size_t BaseShapes::cubeVerticesSize = sizeof(cubeVertices);

// Need to set up indices for this one 2 work
float BaseShapes::pyramidVertices[] = {-0.5f, 0.0f, -0.5f, 0.0f, -1.0f, 0.0f,

                                       0.5f,  0.0f, -0.5f, 0.0f, -1.0f, 0.0f,

                                       0.5f,  0.0f, 0.5f,  0.0f, -1.0f, 0.0f,

                                       -0.5f, 0.0f, 0.5f,  0.0f, -1.0f, 0.0f,

                                       0.0f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f};

const size_t BaseShapes::pyramidVerticesSize = sizeof(pyramidVertices);

} // namespace Tools

} // namespace Moonstone
