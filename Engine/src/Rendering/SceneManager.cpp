#include "Include/SceneManager.h"
#include "Include/BaseShapes.h"
#include "Rendering/Include/RenderingAPI.h"
#include "Rendering/Include/RenderingCommand.h"
#include <memory>

namespace Moonstone
{

namespace Rendering
{

std::shared_ptr<Scene> SceneManager::LoadDefaultScene()
{
    // Scene Setup
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // Shaders Setup
    std::string gridVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.vert";
    std::string gridFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.frag";
    Rendering::Shader defaultGridShader(gridVert.c_str(), gridFrag.c_str());

    std::string cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Rendering::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    // Camera Setup
    glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    std::shared_ptr<Camera> defaultCamera = std::make_shared<Camera>(cameraPos, cameraFront, cameraUp);

    // Lighting Setup
    Lighting defaultLighting;
    float direction = 0.25f;
    glm::vec3 ambient = {};
    glm::vec3 diffuse = {};
    glm::vec3 specular = {};
    bool isActive = true;

    auto dirLight = Lighting::Light(direction, ambient, diffuse, specular, isActive);
    defaultLighting.AddLight(dirLight);

    // Build scene struct
    scene->sceneID = 0;
    scene->shaders.push_back(defaultGridShader);
    scene->shaders.push_back(cubeShader);
    scene->cameras.push_back(defaultCamera);
    scene->activeCamera = defaultCamera;
    scene->lighting = defaultLighting;
    scene->isGridEnabled = true;

    AddModelToScene(scene);

    return scene;
}

std::unique_ptr<Renderer> SceneManager::InitializeSceneRenderer(std::shared_ptr<Scene> scene)
{
    auto renderer = std::make_unique<Renderer>(scene);
    renderer->InitializeScene();
    return renderer;
}

void SceneManager::AddModelToScene(std::shared_ptr<Scene> scene)
{
    // TODO fix this so it isnt just ugly cubes
    // TODO look at whether a vao/vbo for each object is necessary, extra memory waste ig
    unsigned vao = 0, vbo = 0;
    RenderingCommand::InitVertexArray(vao);
    RenderingCommand::InitVertexBuffer(vbo, Tools::BaseShapes::cubeVertices, Tools::BaseShapes::cubeVerticesSize);
    RenderingCommand::InitVertexAttributes(0, 3, RenderingAPI::NumericalDataType::Float,
                                           RenderingAPI::BooleanDataType::False, 6 * sizeof(float), 0);
    RenderingCommand::InitVertexAttributes(1, 3, RenderingAPI::NumericalDataType::Float,
                                           RenderingAPI::BooleanDataType::False, 6 * sizeof(float), 3 * sizeof(float));

    std::string cubeVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.vert";
    std::string cubeFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultcube.frag";
    Rendering::Shader cubeShader(cubeVert.c_str(), cubeFrag.c_str());

    std::stringstream ss;
    ss << "default_cube_" << scene->objects.size();

    Rendering::SceneObject cube = {
        true, vao, vbo, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};

    scene->objects.push_back(cube);
}

} // namespace Rendering

} // namespace Moonstone
