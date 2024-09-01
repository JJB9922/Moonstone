#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <glm/glm.hpp>

namespace Moonstone
{

namespace Rendering
{

class CameraController
{
    public:
        virtual void OnMoveForward()  = 0;
        virtual void OnMoveBackward() = 0;
        virtual void OnMoveLeft()     = 0;
        virtual void OnMoveRight()    = 0;

        virtual float GetPitch() const = 0;
        virtual float GetYaw() const   = 0;
        virtual float GetFov() const   = 0;
        virtual float GetSpeed() const = 0;

        virtual glm::vec3 GetDirection() const = 0;

        virtual bool GetConnected() const = 0;

        virtual void SetFront(const glm::vec3& front) = 0;
        virtual void SetPitch(const float& pitch)     = 0;
        virtual void SetYaw(const float& yaw)         = 0;
        virtual void SetFov(const float& fov)         = 0;
        virtual void SetSpeed(const float& speed)     = 0;

        virtual void SetDirection(const glm::vec3& direction) = 0;

        virtual void SetConnected(const bool& isConnected) = 0;

        virtual ~CameraController() = default;
};

} // namespace Rendering

} // namespace Moonstone

#endif // CAMERACONTROLLER_H
