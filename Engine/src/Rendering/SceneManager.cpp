#include "Include/SceneManager.h"

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

    // Lighting Setup
    Lighting defaultLighting;
    glm::vec3 direction = {0.3f, 0.3f, 0.3f};
    glm::vec3 ambient = {0.3f, 0.3f, 0.3f};
    glm::vec3 diffuse = {0.3f, 0.3f, 0.3f};
    glm::vec3 specular = {0.3f, 0.3f, 0.3f};
    bool isActive = false;

    auto dirLight = Lighting::Light(direction, ambient, diffuse, specular, isActive);
    // defaultLighting.AddLight(dirLight);

    glm::vec3 position = {3.3f, 1.3f, 0.3f};

    auto pointLight = Lighting::Light(position, {0.2f, 0.2f, 0.2f}, {2.0f, 2.0f, 2.0f}, {2.0f, 2.0f, 2.0f}, true, 0.1f,
                                      0.09f, 0.032f);
    defaultLighting.AddLight(pointLight);

    // Build scene struct
    scene->sceneID = 0;
    scene->shaders.push_back(defaultGridShader);
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

    std::string lampCubeVert = std::string(RESOURCE_DIR) + "/Shaders/lightvshader.vs";
    std::string lampCubeFrag = std::string(RESOURCE_DIR) + "/Shaders/lightfshader.fs";
    Rendering::Shader lampCubeShader(lampCubeVert.c_str(), lampCubeFrag.c_str());

    Material material;
    glm::vec3 diffuse = {1.2f, 0.7f, 0.64f};
    glm::vec3 specular = {1.73f, 1.73f, 1.72f};
    float shininess = 64.0f;
    glm::vec3 baseColour = {1.0f, 0.0f, 1.0f};
    auto cubeMat = Material::Mat(diffuse, specular, shininess, baseColour);
    auto lampCubeMat = Material::Mat(diffuse, specular, shininess, {1.0f, 1.0f, 1.0f});

    std::stringstream ss;
    ss << "default_cube_" << scene->objects.size();

    Rendering::SceneObject cube = {true,      vao,     vbo,      {0, 0, 0},  {0, 0, 0},
                                   {1, 1, 1}, cubeMat, ss.str(), cubeShader, Tools::BaseShapes::cubeVerticesSize};

    Rendering::SceneObject lampCube = {true,
                                       vao,
                                       vbo,
                                       {3.3f, 1.3f, 0.3f},
                                       {0, 0, 0},
                                       {0.2f, 0.2f, 0.2f},
                                       lampCubeMat,
                                       ss.str(),
                                       lampCubeShader,
                                       Tools::BaseShapes::cubeVerticesSize};

    scene->objects.push_back(cube);
    scene->objects.push_back(lampCube);
}

} // namespace Rendering

} // namespace Moonstone
