#ifndef EDITORUI_H
#define EDITORUI_H

#include "Core/Include/LayerStack.h"
#include "Core/Include/Window.h"
#include "Core/Layers/Include/BaseLayers.h"
#include "Tools/ImGui/Include/ImGuiLayer.h"
#include "imgui.h"

namespace Moonstone
{

namespace Core
{

class EditorUI
{
    public:
        EditorUI() = default;

        ~EditorUI();

        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);
        void PushOverlay(Layer *overlay);
        void PopOverlay(Layer *overlay);

        inline void SetWindow(std::shared_ptr<Window> window) { m_Window = window; }
        void        Init();
        void        Shutdown();

        inline LayerStack        &GetLayerStack() { return m_LayerStack; }
        inline std::vector<Layer> GetLayers() { return m_Layers; }

    private:
        LayerStack              m_LayerStack;
        std::vector<Layer>      m_Layers;
        std::shared_ptr<Window> m_Window;
};

} // namespace Core

} // namespace Moonstone

#endif // EDITORUI_H
