#include "Include/Renderer.h"
#include "Include/BaseShapes.h"
#include "Include/Logger.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/RenderingCommand.h"
#include "Rendering/Include/Scene.h"
#include "ext/matrix_transform.hpp"
#include "trigonometric.hpp"
#include <memory>
#include <string>

namespace Moonstone
{

namespace Rendering
{

Renderer::Renderer(std::shared_ptr<Scene> scene) : m_Scene(scene)
{
}

void Renderer::InitializeActiveCamera()
{
    m_Window->SetCamera(m_Scene->activeCamera);
}

void Renderer::InitializeScene()
{
    // Initialize an empty scene with a grid
    m_VAO = 0;
    m_VBO = 0;

    RenderingCommand::InitVertexArray(m_VAO);
    RenderingCommand::InitVertexBuffer(m_VBO, Tools::BaseShapes::gridVertices, Tools::BaseShapes::gridVerticesSize);
    RenderingCommand::InitVertexAttributes(0, 3, RenderingAPI::NumericalDataType::Float,
                                           RenderingAPI::BooleanDataType::False, 3 * sizeof(float), 0);
}

void Renderer::InitializeFramebuffer()
{
    int width, height;
    glfwGetWindowSize(m_Window->m_Window, &width, &height);
    RenderingCommand::InitFrameBuffer(width, height, m_FBOTextureMap, m_FBODepthTexture, m_FBO, m_ScreenQuadVAO,
                                      m_ScreenQuadVBO);

    std::string framebVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.vert";
    std::string framebFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.frag";
    Rendering::Shader framebShader(framebVert.c_str(), framebFrag.c_str());
    m_FBShaderID = framebShader.ID;

    m_SceneRenderTarget->SetFramebufferParams(m_FBOTextureMap, m_FBShaderID, m_ScreenQuadVAO);
}

void Renderer::RenderScene()
{
    RenderingCommand::BindFrameBuffer(m_FBO);
    RenderingCommand::EnableDepthTesting();
    RenderingCommand::EnableFaceCulling();
    RenderingCommand::ClearColor(m_Scene->background);
    RenderingCommand::Clear();

    SetupCamera();

    if (m_Scene->isGridEnabled)
    {
        RenderEditorGrid();
    }

    RenderVisibleObjects();

    unsigned int empty = 0;
    RenderingCommand::BindFrameBuffer(empty);
}

void Renderer::SetupCamera()
{
    float nearClip = 0.1f;
    float farClip = 100.0f;

    m_Scene->activeCamera->SetProjectionMatrix(m_Window->GetWidth(), m_Window->GetHeight(), nearClip, farClip);

    m_Scene->activeCamera->SetViewMatrix();
    m_Scene->activeCamera->SetModel({0, 0, 0});
}

void Renderer::RenderEditorGrid()
{
    if (!m_Scene->shaders.empty())
    {
        RenderingCommand::DisableFaceCulling();
        m_Scene->shaders[0].Use();

        RenderingCommand::EnableBlending();
        RenderingCommand::DisableDepthMask();

        if (m_VAO != 0)
        {
            RenderingCommand::BindVertexArray(m_VAO);

            RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "model", m_Scene->activeCamera->GetModel());
            RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "view", m_Scene->activeCamera->GetViewMatrix());
            RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "projection",
                                             m_Scene->activeCamera->GetProjectionMatrix());

            RenderingCommand::SubmitDrawArrays(RenderingAPI::DrawMode::Triangles, 0,
                                               Tools::BaseShapes::gridVerticesSize / 3 * sizeof(float));

            unsigned int empty = 0;
            RenderingCommand::BindVertexArray(empty);
        }
        else
        {
            MS_ERROR("VAO is not set up correctly");
        }

        RenderingCommand::DisableBlending();
        RenderingCommand::EnableDepthMask();
        RenderingCommand::EnableFaceCulling();
    }
    else
    {
        MS_ERROR("shader is not loaded or empty");
    }
}

void Renderer::RenderVisibleObjects()
{
    unsigned currentShaderID = 0;

    for (auto object : m_Scene->objects)
    {
        if (object.shader.ID != currentShaderID)
        {
            object.shader.Use();
            currentShaderID = object.shader.ID;

            RenderLighting(object);
        }

        RenderingCommand::BindVertexArray(object.vao);

        // User applied transformations
        glm::mat4 modelTransformationMatrix =
            glm::translate(glm::mat4(1.0f), object.position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(object.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(object.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), glm::radians(object.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
            glm::scale(glm::mat4(1.0f), object.scale);

        RenderingCommand::SetUniformMat4(object.shader.ID, "model", modelTransformationMatrix);
        RenderingCommand::SetUniformMat4(object.shader.ID, "view", m_Scene->activeCamera->GetViewMatrix());
        RenderingCommand::SetUniformMat4(object.shader.ID, "projection", m_Scene->activeCamera->GetProjectionMatrix());

        // If an object doesn't have a material or texture, make it the ugliest pink u can possibly imagine
        RenderingCommand::SetUniformVec3(object.shader.ID, "material.baseColour", object.material.baseColour);
        RenderingCommand::SetUniformVec3(object.shader.ID, "material.diffuse", object.material.diffuse);
        RenderingCommand::SetUniformVec3(object.shader.ID, "material.specular", object.material.specular);
        RenderingCommand::SetUniformFloat(object.shader.ID, "material.shininess", object.material.shininess);

        RenderingCommand::SetUniformVec3(object.shader.ID, "viewPos", m_Scene->activeCamera->GetPosition());

        // TODO Set to time of day or user set dirlight
        // TODO Fix for a clean blend between cubes and models
        RenderingCommand::SubmitDrawArrays(RenderingAPI::DrawMode::Triangles, 0, object.size);

        unsigned int empty = 0;
        RenderingCommand::BindVertexArray(empty);
    }
}

void Renderer::RenderLighting(SceneObject &object)
{

    auto lights = m_Scene->lights;
    std::vector<Lighting::Light> pointLights;

    for (auto light : lights)
    {
        if (light.type == Lighting::LightType::Directional)
        {
            RenderingCommand::SetUniformVec3(object.shader.ID, "dirLight.direction", light.direction);
            RenderingCommand::SetUniformVec3(object.shader.ID, "dirLight.ambient", light.ambient);
            RenderingCommand::SetUniformVec3(object.shader.ID, "dirLight.diffuse", light.diffuse);
            RenderingCommand::SetUniformVec3(object.shader.ID, "dirLight.specular", light.specular);
            RenderingCommand::SetUniformBool(object.shader.ID, "dirLight.isActive", light.isActive);
        }
        else if (light.type == Lighting::LightType::Point)
        {
            pointLights.push_back(light);
        }
    }

    // TODO SSBO
    for (unsigned i = 0; i < pointLights.size(); ++i)
    {
        std::stringstream ss;
        ss << "pointLights[" << i << "].position";
        RenderingCommand::SetUniformVec3(object.shader.ID, ss.str(), pointLights[i].position);

        ss.str("");
        ss.clear(); // Clear the stringstream for reuse
        ss << "pointLights[" << i << "].ambient";
        RenderingCommand::SetUniformVec3(object.shader.ID, ss.str(), pointLights[i].ambient);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].diffuse";
        RenderingCommand::SetUniformVec3(object.shader.ID, ss.str(), pointLights[i].diffuse);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].specular";
        RenderingCommand::SetUniformVec3(object.shader.ID, ss.str(), pointLights[i].specular);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].constant";
        RenderingCommand::SetUniformFloat(object.shader.ID, ss.str(), pointLights[i].constant);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].linear";
        RenderingCommand::SetUniformFloat(object.shader.ID, ss.str(), pointLights[i].linear);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].quadratic";
        RenderingCommand::SetUniformFloat(object.shader.ID, ss.str(), pointLights[i].quadratic);

        ss.str("");
        ss.clear();
        ss << "pointLights[" << i << "].isActive";
        RenderingCommand::SetUniformFloat(object.shader.ID, ss.str(), pointLights[i].isActive);
    }
}

void Renderer::CleanupScene()
{
    for (int i = 0; i < m_Scene->objects.size(); ++i)
    {
        RenderingCommand::Cleanup(m_VAO, m_VBO, m_Scene->shaders[i].ID);
    }
}

} // namespace Rendering

} // namespace Moonstone
