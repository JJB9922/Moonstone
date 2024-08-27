#ifndef SCENE_H
#define SCENE_H

#include "Renderer/Include/Shader.h"
#include "mspch.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Renderer
{

class Scene
{
    public:
        struct SceneObject
        {
                bool isActive;

                glm::vec3 position = {0.0f, 0.0f, 0.0f};

                std::string      name;
                Renderer::Shader shader;
                size_t           size;
        };
};

} // namespace Renderer

} // namespace Moonstone

#endif // SCENE_H
