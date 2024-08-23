#ifndef CAMERA_H
#define CAMERA_H

#include "Renderer/Include/CameraController.h"

#include "Core/Include/Logger.h"
#include "Core/Include/Time.h"

namespace Moonstone
{

namespace Renderer
{

class Camera : public CameraController
{
        Core::Time& time = Core::Time::GetInstance();

    public:
        /**
         * @brief Camera Initialize a camera
         * @param pos
         * @param front
         * @param up
         */
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

        glm::vec3 GetPosition() const { return m_CameraPos; }
        glm::vec3 GetFront() const { return m_CameraFront; }
        glm::vec3 GetUp() const { return m_CameraUp; }

        float     GetPitch() const override { return m_CameraPitch; }
        float     GetYaw() const override { return m_CameraYaw; }
        float     GetFov() const override { return m_CameraFov; }
        float     GetSpeed() const override { return m_CameraSpeed; }
        glm::vec3 GetDirection() const override { return m_CameraDirection; }

        bool GetConnected() const override { return m_CameraConnected; }

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
        glm::vec3 m_CameraPos;
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraUp;
        glm::vec3 m_CameraDirection;
        float     m_CameraPitch     = 0.0f;
        float     m_CameraYaw       = -90.0f;
        float     m_CameraFov       = 65.0f;
        float     m_CameraSpeed     = 10.0f;
        bool      m_CameraConnected = true;
};

} // namespace Renderer

} // namespace Moonstone

#endif // CAMERA_H
