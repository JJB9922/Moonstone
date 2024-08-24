#ifndef TEXTURES_H
#define TEXTURES_H

#include "stb_image.h"

namespace Moonstone
{

namespace Renderer
{

class Textures
{
    public:
        Textures();

        static inline unsigned char *LoadTexture(const char *texPath, int &width, int &height, int &nrChannels)
        {
            return stbi_load(texPath, &width, &height, &nrChannels, 0);
        }
};

} // namespace Renderer

} // namespace Moonstone

#endif // TEXTURES_H
