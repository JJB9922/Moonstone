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
            Exit,
            ApplyBGColor
        };

        using ButtonCallback = std::function<void()>;

        ExampleLayer()
            : Layer("Example")
        {
        }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }

        inline ImVec4 GetBGColor() { return m_BGColor; }
        void          SetBGColor(const ImVec4& color) { m_BGColor = color; }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            ImGui::Begin("Moonstone");
            ImGui::Text("Moonstone");

            if (ImGui::Button("Exit", ImVec2(150, 40)) && m_BtnCallbacks[ButtonID::Exit])
            {
                m_BtnCallbacks[ButtonID::Exit]();
            }

            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
            ImGui::Text("Background Color:");

            static bool alpha_preview      = true;
            static bool alpha_half_preview = false;
            static bool drag_and_drop      = true;
            static bool options_menu       = true;
            static bool hdr                = false;

            ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0)
                                             | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop)
                                             | (alpha_half_preview
                                                    ? ImGuiColorEditFlags_AlphaPreviewHalf
                                                    : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0))
                                             | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

            if (ImGui::ColorEdit4("##2f", (float*) &color, ImGuiColorEditFlags_Float | misc_flags))
            {
                SetBGColor(color);
            }

            if (ImGui::Button("Apply Colour", ImVec2(150, 40)) && m_BtnCallbacks[ButtonID::ApplyBGColor])
            {
                m_BtnCallbacks[ButtonID::ApplyBGColor]();
            }

            ImGui::End();
        };

    private:
        std::unordered_map<ButtonID, ButtonCallback> m_BtnCallbacks;
        ImVec4                                       m_BGColor;
};

} // namespace Core

} // namespace Moonstone

#endif // BASELAYERS_H
