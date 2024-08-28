#ifndef CAMERA_H
#define CAMERA_H

#include "Renderer/Include/CameraController.h"

#include "Core/Include/Logger.h"
#include "Core/Include/Time.h"
#include "Renderer/Include/RendererCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moonstone
{

namespace Renderer
{

class Camera : public CameraController
{
        Core::Time& time = Core::Time::GetInstance();

    public:
        Camera(const glm::vec3& pos, const glm::vec3& front, const glm::vec3& up)
            : m_CameraPos(pos)
            , m_CameraFront(front)
            , m_CameraUp(up)
        {
            Core::Time& time = Core::Time::GetInstance();
        }

        void OnMoveForward() override { m_CameraPos += m_CameraSpeed * time.GetDeltaTime() * m_CameraFront; }

        void OnMoveBackward() override { m_CameraPos -= m_CameraSpeed * time.GetDeltaTime() * m_CameraFront; }

        void OnMoveLeft() override
        {
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * time.GetDeltaTime();
        }

        void OnMoveRight() override
        {
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * m_CameraSpeed * time.GetDeltaTime();
        }

        inline glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
        inline glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
        inline glm::mat4 GetModel() const { return m_Model; }

        inline glm::vec3 GetPosition() const { return m_CameraPos; }
        inline glm::vec3 GetFront() const { return m_CameraFront; }
        inline glm::vec3 GetUp() const { return m_CameraUp; }

        inline float     GetPitch() const override { return m_CameraPitch; }
        inline float     GetYaw() const override { return m_CameraYaw; }
        inline float     GetFov() const override { return m_CameraFov; }
        inline float     GetSpeed() const override { return m_CameraSpeed; }
        inline glm::vec3 GetDirection() const override { return m_CameraDirection; }

        inline bool GetConnected() const override { return m_CameraConnected; }

        void SetProjectionMatrix(unsigned                shaderID,
                                 std::shared_ptr<Camera> camera,
                                 int                     winWidth,
                                 int                     winHeight,
                                 float                   nearClip,
                                 float                   farClip);
        void SetViewMatrix(unsigned shaderID, std::shared_ptr<Camera> camera);
        void SetModel(unsigned shaderID, glm::vec3 model);
        void SetModelTransform(unsigned shaderID, glm::mat4 model);

        void SetPosition(const glm::vec3& pos) { m_CameraPos = pos; }
        void SetFront(const glm::vec3& front) override { m_CameraFront = front; }
        void SetUp(const glm::vec3& up) { m_CameraUp = up; }

        void SetPitch(const float& pitch) override { m_CameraPitch = pitch; }
        void SetYaw(const float& yaw) override { m_CameraYaw = yaw; }
        void SetFov(const float& fov) override { m_CameraFov = fov; }
        void SetSpeed(const float& speed) override { m_CameraSpeed = speed; }
        void SetDirection(const glm::vec3& direction) override { m_CameraDirection = direction; }

        void SetConnected(const bool& isConnected) override { m_CameraConnected = isConnected; }

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_Model;

        glm::vec3 m_CameraPos;
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraUp;
        glm::vec3 m_CameraDirection;
        float     m_CameraPitch = 0.0f;
        float     m_CameraYaw   = -90.0f;
        float     m_CameraFov   = 65.0f;
        float     m_CameraSpeed = 30.0f;

        bool m_CameraConnected = true;
};

} // namespace Renderer

} // namespace Moonstone

#endif // CAMERA_H
