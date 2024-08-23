#include "Include/Camera.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Sets the projection matrix for the given shader.
 *
 * Calculates the perspective projection matrix based on the camera's field of view (FOV), 
 * the aspect ratio of the window, and the near and far clipping planes. This matrix is then 
 * set as a uniform in the specified shader.
 *
 * @param shaderID The ID of the shader program where the projection matrix will be set.
 * @param camera A shared pointer to the Camera object containing the FOV and other parameters.
 * @param winWidth The width of the window or viewport.
 * @param winHeight The height of the window or viewport.
 * @param nearClip The distance to the near clipping plane.
 * @param farClip The distance to the far clipping plane.
 */
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

/**
 * @brief Sets the view matrix for the given shader.
 *
 * Calculates the view matrix based on the camera's position, front vector (direction), and up vector. 
 * This matrix represents the camera's orientation and position in the world. The matrix is then 
 * set as a uniform in the specified shader.
 *
 * @param shaderID The ID of the shader program where the view matrix will be set.
 * @param camera A shared pointer to the Camera object containing the position, front, and up vectors.
 */
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
