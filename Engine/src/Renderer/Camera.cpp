#include "Include/Camera.h"

namespace Moonstone
{

namespace Renderer
{

void Camera::SetProjectionMatrix(
    unsigned shaderID, std::shared_ptr<Camera> camera, int winWidth, int winHeight, float nearClip, float farClip)
{
    glm::mat4 projection = glm::mat4(1.0);

    projection = glm::perspective(glm::radians(camera->GetFov()),
                                  (float) winWidth / (float) winHeight,
                                  nearClip,
                                  farClip);

    RendererCommand::SetUniformMat4(shaderID, "projection", projection);

    m_ProjectionMatrix = projection;
}

void Camera::SetViewMatrix(unsigned shaderID, std::shared_ptr<Camera> camera)
{
    glm::mat4 view;
    view = glm::lookAt(camera->GetPosition(), camera->GetPosition() + camera->GetFront(), camera->GetUp());

    Renderer::RendererCommand::SetUniformMat4(shaderID, "view", view);

    m_ViewMatrix = view;
}

void Camera::SetModel(unsigned shaderID, glm::vec3 model)
{
    glm::mat4 m = glm::mat4(1.0f);
    m           = glm::translate(m, model);

    RendererCommand::SetUniformMat4(shaderID, "model", m);

    m_Model = m;
}

void Camera::SetModelTransform(unsigned shaderID, glm::mat4 model)
{
    RendererCommand::SetUniformMat4(shaderID, "model", model);

    m_Model = model;
}

} // namespace Renderer

} // namespace Moonstone
