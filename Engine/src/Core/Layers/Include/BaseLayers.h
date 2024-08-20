#ifndef BASELAYERS_H
#define BASELAYERS_H

#include "Core/Include/Layer.h"
#include "imgui.h"

namespace Moonstone
{

namespace Core
{
class ExampleLayer : public Layer
{
    public:
        enum class ButtonID
        {
            Exit
        };

        using ButtonCallback = std::function<void()>;

        ExampleLayer()
            : Layer("Example")
        {
        }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            ImGui::Begin("Moonstone");
            ImGui::Text("Moonstone");

            if (ImGui::Button("Exit", ImVec2(150, 40)) && m_BtnCallbacks[ButtonID::Exit])
            {
                m_BtnCallbacks[ButtonID::Exit]();
            }

            ImGui::End();
        };

    private:
        std::unordered_map<ButtonID, ButtonCallback> m_BtnCallbacks;
};

} // namespace Core

} // namespace Moonstone

#endif // BASELAYERS_H
