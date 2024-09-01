#ifndef SCENE_H
#define SCENE_H

#include "Rendering/Include/Camera.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/Shader.h"

namespace Moonstone
{

namespace Rendering
{

struct SceneObject
{
        bool isActive;

        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
        glm::vec3 scale    = {1.0f, 1.0f, 1.0f};

        std::string       name;
        Rendering::Shader shader;
        size_t            size;

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

struct Scene
{
        int                      sceneID  = 0;
        std::vector<Shader>      shaders  = {};
        std::vector<Camera>      cameras  = {};
        std::vector<SceneObject> objects  = {};
        Lighting                 lighting = {};

        glm::vec4 background    = {0.15f, 0.15f, 0.15f, 1.0f};
        bool isGridEnabled = true;

        Scene()  = default;
        ~Scene() = default;
};

} // namespace Rendering

} // namespace Moonstone

#endif // SCENE_H