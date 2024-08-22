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

        /**
         * @brief Load a texture from file using STB
         * @param texPath
         * @param width
         * @param height
         * @param nrChannels
         * @return 
         */
        static inline unsigned char *LoadTexture(const char *texPath, int &width, int &height, int &nrChannels)
        {
            return stbi_load(texPath, &width, &height, &nrChannels, 0);
        }
};

} // namespace Renderer

} // namespace Moonstone

#endif // TEXTURES_H
