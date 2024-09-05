#include "Include/Camera.h"

namespace Moonstone
{

namespace Rendering
{

void Camera::SetProjectionMatrix(int winWidth, int winHeight, float nearClip, float farClip)
{
    glm::mat4 projection = glm::mat4(1.0);

    projection = glm::perspective(glm::radians(GetFov()), (float)winWidth / (float)winHeight, nearClip, farClip);

    m_ProjectionMatrix = projection;
}

void Camera::SetViewMatrix()
{
    glm::mat4 view;
    view = glm::lookAt(GetPosition(), GetPosition() + GetFront(), GetUp());

    m_ViewMatrix = view;
}

void Camera::SetModel(glm::vec3 model)
{
    glm::mat4 m = glm::mat4(1.0f);
    m = glm::translate(m, model);

    m_Model = m;
}

void Camera::SetModelTransform(unsigned shaderID, glm::mat4 model)
{
    RenderingCommand::SetUniformMat4(shaderID, "model", model);

    m_Model = model;
}

} // namespace Rendering

} // namespace Moonstone
