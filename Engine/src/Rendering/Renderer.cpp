#include "Include/Renderer.h"
#include "Include/BaseShapes.h"
#include "Rendering/Include/RenderingCommand.h"
#include <memory>

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

    Rendering::RenderingCommand::InitVertexArray(m_VAO);
    Rendering::RenderingCommand::InitVertexBuffer(m_VBO, Tools::BaseShapes::gridVertices,
                                                  Tools::BaseShapes::gridVerticesSize);
    Rendering::RenderingCommand::InitVertexAttributes(0, 3, Rendering::RenderingAPI::NumericalDataType::Float,
                                                      Rendering::RenderingAPI::BooleanDataType::False,
                                                      3 * sizeof(float), 0);
}

void Renderer::InitializeFramebuffer()
{
    int width, height;
    glfwGetWindowSize(m_Window->m_Window, &width, &height);
    Rendering::RenderingCommand::InitFrameBuffer(width, height, m_FBOTextureMap, m_FBODepthTexture, m_FBO,
                                                 m_ScreenQuadVAO, m_ScreenQuadVBO);

    std::string framebVert = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.vert";
    std::string framebFrag = std::string(RESOURCE_DIR) + "/Shaders/DefaultShapes/defaultfbo.frag";
    Rendering::Shader framebShader(framebVert.c_str(), framebFrag.c_str());
    m_FBShaderID = framebShader.ID;

    m_SceneRenderTarget->SetFramebufferParams(m_FBOTextureMap, m_FBShaderID, m_ScreenQuadVAO);
}

void Renderer::RenderScene()
{
    Rendering::RenderingCommand::BindFrameBuffer(m_FBO);
    Rendering::RenderingCommand::EnableDepthTesting();
    Rendering::RenderingCommand::EnableFaceCulling();
    Rendering::RenderingCommand::ClearColor(m_Scene->background);
    Rendering::RenderingCommand::Clear();

    RenderCamera();

    if (m_Scene->isGridEnabled)
    {
        RenderEditorGrid();
    }

    unsigned int empty = 0;
    Rendering::RenderingCommand::BindFrameBuffer(empty);
}

void Renderer::RenderCamera()
{
    float nearClip = 0.1f;
    float farClip = 100.0f;

    m_Scene->activeCamera->SetProjectionMatrix(m_Scene->activeCamera, m_Window->GetWidth(), m_Window->GetHeight(),
                                               nearClip, farClip);

    m_Scene->activeCamera->SetViewMatrix(m_Scene->activeCamera);
    m_Scene->activeCamera->SetModel({0, 0, 0});
}

void Renderer::RenderEditorGrid()
{
    if (!m_Scene->shaders.empty())
    {
        Rendering::RenderingCommand::DisableFaceCulling();
        m_Scene->shaders[0].Use();

        Rendering::RenderingCommand::EnableBlending();
        Rendering::RenderingCommand::DisableDepthMask();

        if (m_VAO != 0)
        {
            Rendering::RenderingCommand::BindVertexArray(m_VAO);

            Rendering::RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "model",
                                                        m_Scene->activeCamera->GetModel());
            Rendering::RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "view",
                                                        m_Scene->activeCamera->GetViewMatrix());
            Rendering::RenderingCommand::SetUniformMat4(m_Scene->shaders[0].ID, "projection",
                                                        m_Scene->activeCamera->GetProjectionMatrix());

            Rendering::RenderingCommand::SubmitDrawArrays(Rendering::RenderingAPI::DrawMode::Triangles, 0,
                                                          Tools::BaseShapes::gridVerticesSize / 3 * sizeof(float));
        }
        else
        {
            MS_ERROR("VAO is not set up correctly");
        }

        Rendering::RenderingCommand::DisableBlending();
        Rendering::RenderingCommand::EnableDepthMask();
        Rendering::RenderingCommand::EnableFaceCulling();
    }
    else
    {
        MS_ERROR("shader is not loaded or empty");
    }
}

void Renderer::CleanupScene()
{
    for (int i = 0; i < m_Scene->objects.size(); ++i)
    {
        Rendering::RenderingCommand::Cleanup(m_VAO, m_VBO, m_Scene->shaders[i].ID);
    }
}

} // namespace Rendering

} // namespace Moonstone
