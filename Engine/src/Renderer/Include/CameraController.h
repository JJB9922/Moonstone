#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

namespace Moonstone
{

namespace Renderer
{

class CameraController
{
    public:
        virtual void OnMoveForward()  = 0;
        virtual void OnMoveBackward() = 0;
        virtual void OnMoveLeft()     = 0;
        virtual void OnMoveRight()    = 0;

        virtual ~CameraController() = default;
};

} // namespace Renderer

} // namespace Moonstone

#endif // CAMERACONTROLLER_H
