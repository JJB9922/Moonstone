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
                glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
                glm::vec3 scale    = {1.0f, 1.0f, 1.0f};

                std::string      name;
                Renderer::Shader shader;
                size_t           size;

                void Clear()
                {
                    isActive                     = false;
                    position                     = {0, 0, 0};
                    rotation                     = {0, 0, 0};
                    scale                        = {1, 1, 1};
                    name                         = "";
                    std::optional<Shader> shader = std::nullopt;
                    size                         = 0;
                }
        };
};

} // namespace Renderer

} // namespace Moonstone

#endif // SCENE_H
