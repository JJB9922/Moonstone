#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

#include "Core/Include/Logger.h"
#include "Core/Include/Time.h"
#include "Renderer/Include/CameraController.h"

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
            , m_CameraSpeed(10.0f)
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

        void SetPosition(const glm::vec3& pos) { m_CameraPos = pos; }
        void SetFront(const glm::vec3& front) { m_CameraFront = front; }
        void SetUp(const glm::vec3& up) { m_CameraUp = up; }

    private:
        glm::vec3 m_CameraPos;
        glm::vec3 m_CameraFront;
        glm::vec3 m_CameraUp;
        float     m_CameraSpeed;
};

} // namespace Renderer

} // namespace Moonstone

#endif // CAMERA_H
