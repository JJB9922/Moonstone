#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Include/Core.h"
#include "Core/Include/Logger.h"
#include "Core/Include/Window.h"

namespace Moonstone
{

namespace Core
{

class Application
{
public:
    Application();
    virtual ~Application() = default;

    void Run();

    inline static Application &GetApplicationInstance() { return *s_ApplicationInstance; }

    void PushLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PushOverlay(Layer *overlay);
    void PopOverlay(Layer *overlay);

private:
    void RenderLayers();
    void InitializeImGui();
    void InitializeTestRenderData(unsigned &shaderProgram, unsigned &VBO, unsigned &VAO, unsigned &EBO);

private:
    bool                    m_Running;
    static Application     *s_ApplicationInstance;
    LayerStack              m_LayerStack;
    std::unique_ptr<Window> m_Window;
    Tools::ImGuiLayer      *m_ImGuiLayer;
};

Application* CreateApplicationInstance();

} // namespace Core

} // namespace Moonstone

#endif // APPLICATION_H