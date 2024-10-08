#include "Include/SceneManager.h"
#include "Include/Logger.h"
#include "Rendering/Include/Model.h"
#include <string>

namespace Moonstone
{

namespace Rendering
{

std::shared_ptr<Scene> SceneManager::LoadDefaultScene()
{
    // Scene Setup
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    // Shader Setup
    std::string gridVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.vert";
    std::string gridFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultgrid.frag";
    Rendering::Shader defaultGridShader(gridVert.c_str(), gridFrag.c_str());

    // Camera Setup
    glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 20.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    std::shared_ptr<Camera> defaultCamera = std::make_shared<Camera>(cameraPos, cameraFront, cameraUp);

    // Build scene struct
    scene->sceneID = 0;
    scene->shaders.push_back(defaultGridShader);
    scene->cameras.push_back(defaultCamera);
    scene->activeCamera = defaultCamera;
    scene->isGridEnabled = true;

    return scene;
}

std::unique_ptr<Renderer> SceneManager::InitializeSceneRenderer(std::shared_ptr<Scene> scene)
{
    auto renderer = std::make_unique<Renderer>(scene);
    renderer->InitializeScene();
    return renderer;
}

void SceneManager::AddLightToScene(std::shared_ptr<Scene> scene, Lighting::Light &light)
{
    if (light.type == Lighting::LightType::Point)
    {
        scene->lights.emplace_back(light.id, light.position, light.ambient, light.diffuse, light.specular,
                                   light.isActive, light.constant, light.linear, light.quadratic);
    }
    else if (light.type == Lighting::LightType::Directional)
    {
        scene->lights.emplace_back(light.id, light.direction, light.ambient, light.diffuse, light.specular,
                                   light.isActive);
    }
}

void SceneManager::AddObjectToScene(std::shared_ptr<Scene> scene)
{
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

    Material material;
    glm::vec3 diffuse = {1.2f, 0.7f, 0.64f};
    glm::vec3 specular = {1.73f, 1.73f, 1.72f};
    float shininess = 64.0f;
    glm::vec3 baseColour = {1.0f, 0.0f, 1.0f};
    auto cubeMat = Material::Mat(diffuse, specular, shininess, baseColour);

    std::stringstream ss;
    ss << "default_cube_" << scene->objects.size();

    Rendering::SceneObject cube = {true,      vao,     vbo,      {0, 0, 0},  {0, 0, 0},
                                   {1, 1, 1}, cubeMat, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};

    scene->objects.push_back(cube);
}

void SceneManager::AddModelToScene(std::shared_ptr<Scene> scene)
{
    stbi_set_flip_vertically_on_load(true);
    std::string modelVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultmesh.vert";
    std::string modelFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultmesh.frag";
    Rendering::Shader modelShader(modelVert.c_str(), modelFrag.c_str());

    std::stringstream ss;
    ss << "model_" << scene->models.size();

    //  TODO NO
    auto  defM = std::string(RESOURCE_DIR) + "/Models/backpack/backpack.obj";
    Model model(ss.str(), modelShader, {0, 0, 0}, {0, 0, 0}, {1, 1, 1}, defM);

    scene->models.push_back(model);
}

} // namespace Rendering

} // namespace Moonstone
