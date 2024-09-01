#ifndef BASESHAPES_H
#define BASESHAPES_H

#include "mspch.h"

namespace Moonstone
{

namespace Tools
{

class BaseShapes
{
    public:
        static float gridVertices[];
        static float cubeVertices[];
        static float pyramidVertices[];
        static float screenQuadVertices[];

        static const size_t gridVerticesSize;
        static const size_t cubeVerticesSize;
        static const size_t pyramidVerticesSize;
        static const size_t screenQuadVerticesSize;
};

} // namespace Tools

} // namespace Moonstone

#endif // BASESHAPES_H
