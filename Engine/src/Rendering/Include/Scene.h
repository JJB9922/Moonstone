#ifndef SCENE_H
#define SCENE_H

#include "Rendering/Include/Camera.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/Material.h"
#include "Rendering/Include/Shader.h"
#include <memory>

namespace Moonstone
{

namespace Rendering
{

struct SceneObject
{
    bool isActive;

    unsigned vao = 0, vbo = 0;

    glm::vec3 position = {0.0f, 0.0f, 0.0f};
    glm::vec3 rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    Material::Mat material;

    std::string name;
    Rendering::Shader shader;
    size_t size;

    void Clear()
    {
        isActive = false;
        vao = 0;
        vbo = 0;
        position = {0, 0, 0};
        rotation = {0, 0, 0};
        scale = {1, 1, 1};
        name = "";
        std::optional<Shader> shader = std::nullopt;
        size = 0;
    }
};

struct Scene
{
    int sceneID = 0;
    std::vector<Shader> shaders = {};
    std::vector<std::shared_ptr<Camera>> cameras = {};
    std::shared_ptr<Camera> activeCamera = nullptr;
    std::vector<SceneObject> objects = {};
    Lighting lighting = {};

    glm::vec4 background = {0.15f, 0.15f, 0.15f, 1.0f};
    bool isGridEnabled = true;

    Scene() = default;
    ~Scene() = default;
};

} // namespace Rendering

} // namespace Moonstone

#endif // SCENE_H
