#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "Core/Include/Layer.h"

namespace Moonstone
{

namespace Tools
{

class ImGuiLayer : public Core::Layer
{
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void SetWindow(GLFWwindow* window);

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Start();
        void End();

    private:
        GLFWwindow* m_Window;
        float m_Time = 0.0f;
};

} // namespace Tools

} // namespace Moonstone

#endif // IMGUILAYER_H
