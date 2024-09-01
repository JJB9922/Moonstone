#include "Include/SceneManager.h"

namespace Moonstone
{

namespace Rendering
{

std::shared_ptr<Scene> SceneManager::LoadDefaultScene()
{
    // Scene Setup
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // Shaders Setup
    std::string      gridVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.vert";
    std::string      gridFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.frag";
    Rendering::Shader defaultGridShader(gridVert.c_str(), gridFrag.c_str());

    std::string      cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string      cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Rendering::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    // Camera Setup
    glm::vec3 cameraPos   = glm::vec3(0.0f, 10.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
    Camera    defaultCamera(cameraPos, cameraFront, cameraUp);

    // Objects Setup
    Rendering::SceneObject defaultCube
        = {true, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, "defaultCube", cubeShader, Tools::BaseShapes::cubeVerticesSize};

    // Lighting Setup
    Lighting  defaultLighting;
    float     direction = 0.25f;
    glm::vec3 ambient   = {};
    glm::vec3 diffuse   = {};
    glm::vec3 specular  = {};
    bool      isActive  = true;

    auto dirLight = Lighting::Light(direction, ambient, diffuse, specular, isActive);
    defaultLighting.AddLight(dirLight);

    // Build scene struct
    scene->sceneID = 0;
    scene->shaders.push_back(defaultGridShader);
    scene->cameras.push_back(defaultCamera);
    scene->objects.push_back(defaultCube);
    scene->lighting      = defaultLighting;
    scene->isGridEnabled = true;

    return scene;
}

void SceneManager::RenderScene(std::shared_ptr<Scene> scene)
{
    Renderer renderer;
    renderer.RenderScene(scene);
}

} // namespace Rendering

} // namespace Moonstone
