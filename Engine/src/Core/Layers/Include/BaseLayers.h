#ifndef BASELAYERS_H
#define BASELAYERS_H

#include "Core/Include/Layer.h"
#include "Core/Include/Time.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

#include <ImGuizmo.h>
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Core
{

class ControlsLayer : public Layer
{
    public:
        enum class ButtonID
        {
            Exit,
            ApplyBGColor,
            ToggleWireframe,
            ApplyCameraSens,
            ToggleGrid,
            AddObject,
            ToggleSunlight,
            ApplyTimeOfDay,
        };

        enum class SliderID
        {
            TimeOfDay,
        };

        enum class SceneObject
        {
            None,
            Cube,
            Sphere,
            Pyramid
        };

        using SliderCallback = std::function<void(float)>;
        using ButtonCallback = std::function<void()>;

        ControlsLayer()
            : Layer("Controls")
        {
        }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }
        void SetSliderCallback(SliderID sliderID, SliderCallback callback) { m_SliderCallbacks[sliderID] = callback; }

        inline ImVec4      GetBGColor() { return m_BGColor; }
        inline void        SetBGColor(const ImVec4& color) { m_BGColor = color; }
        inline float       GetCamSensitivity() { return m_CamSensitivity; }
        inline SceneObject GetAddObject() { return m_AddObject; }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            ImVec2      btnSize  = ImVec2(150, 20);
            ImGuiStyle& style    = ImGui::GetStyle();
            style.FrameRounding  = 2;
            style.WindowRounding = 2;

            ImGui::SetNextWindowPos({0, 100}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({300, 600}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Moonstone");
            ImGui::Text("Moonstone");

            if (ImGui::Button("Exit", btnSize) && m_BtnCallbacks[ButtonID::Exit])
            {
                m_BtnCallbacks[ButtonID::Exit]();
            }

            ImGui::SeparatorText("Engine | Editor Settings");

            //static ImVec4 color = ImVec4(0.7f, 0.75f, 0.78f, 1.0f);
            static ImVec4 color = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
            m_BGColor           = color;
            ImGui::Text("Background Color");

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

            if (ImGui::Button("Apply Colour", btnSize) && m_BtnCallbacks[ButtonID::ApplyBGColor])

            {
                m_BtnCallbacks[ButtonID::ApplyBGColor]();

                MS_DEBUG("background colour changed: r({0}), g({1}), b({2})", color.x, color.y, color.z);
            }

            ImGui::Text("Grid");
            bool gridEnabled = true;
            if (ImGui::Button("Toggle Grid", btnSize) && m_BtnCallbacks[ButtonID::ToggleGrid])

            {
                m_BtnCallbacks[ButtonID::ToggleGrid]();

                MS_DEBUG("grid toggled: {0}", gridEnabled);
            }

            ImGui::Text("Camera Sensitivity");

            ImGui::SliderFloat("Sensitivity", &m_CamSensitivity, 0.0f, 1.0f, "%.3f");

            if (ImGui::Button("Apply Sensitivity", btnSize) && m_BtnCallbacks[ButtonID::ApplyCameraSens])
            {
                m_BtnCallbacks[ButtonID::ApplyCameraSens]();

                MS_DEBUG("camera sens changed: {0}", m_CamSensitivity);
            }

            ImGui::SeparatorText("Renderer:");

            if (ImGui::Button("Toggle Wireframe", btnSize) && m_BtnCallbacks[ButtonID::ToggleWireframe])
            {
                m_BtnCallbacks[ButtonID::ToggleWireframe]();
            }

            ImGui::Text("Lighting");

            if (ImGui::Button("Toggle Sun Light", btnSize) && m_BtnCallbacks[ButtonID::ToggleSunlight])
            {
                m_SunOn = !m_SunOn;
                m_BtnCallbacks[ButtonID::ToggleSunlight]();
            }
            if (m_SunOn)
            {
                ImGui::SameLine();
                ImGui::Text("Sunlight is On");

                ImGui::Text("Time of Day");

                ImGui::SliderFloat("Time of Day", &m_TimeOfDay, 0.0f, 1.0f, "%.3f");

                if (m_SliderCallbacks[SliderID::TimeOfDay])
                {
                    m_SliderCallbacks[SliderID::TimeOfDay](m_TimeOfDay);
                }
            }
            else
            {
                ImGui::SameLine();
                ImGui::Text("Sunlight is Off");
            }

            ImGui::Text("Objects");

            static int  selected_object = -1;
            const char* names[]         = {"Cube", "Sphere", "Pyramid"};

            if (ImGui::Button("Select Object"))
                ImGui::OpenPopup("shape_popup");
            ImGui::SameLine();
            ImGui::TextUnformatted(selected_object == -1 ? "<No Object Selected>" : names[selected_object]);
            if (ImGui::BeginPopup("shape_popup"))
            {
                ImGui::SeparatorText("Base Shapes");
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    if (ImGui::Selectable(names[i]))
                    {
                        selected_object = i;
                    }
                ImGui::EndPopup();

                m_AddObject = static_cast<SceneObject>(selected_object + 1);
            }

            if (ImGui::Button("Add Object", btnSize) && m_BtnCallbacks[ButtonID::AddObject])
            {
                m_BtnCallbacks[ButtonID::AddObject]();
            }

            ImGui::End();
        }

    private:
        bool                                         m_SunOn = false;
        std::unordered_map<ButtonID, ButtonCallback> m_BtnCallbacks;
        std::unordered_map<SliderID, SliderCallback> m_SliderCallbacks;
        ImVec4                                       m_BGColor;
        float                                        m_CamSensitivity = 0.2f;
        float                                        m_TimeOfDay      = 0.5f;
        float                                        m_CamSpeed       = 10.0f;
        SceneObject                                  m_AddObject;
};

class DebugLayer : public Layer
{
    public:
        DebugLayer()
            : Layer("Debug")
        {
        }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            Time& time = Time::GetInstance();

            ImVec2      btnSize  = ImVec2(150, 20);
            ImGuiStyle& style    = ImGui::GetStyle();
            style.FrameRounding  = 2;
            style.WindowRounding = 2;

            ImGui::SetNextWindowPos({0, 0}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({300, 100}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Debug");
            float fps = 1.0f / time.GetDeltaTime();

            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Delta Time: %.4f seconds", time.GetDeltaTime());

            ImGui::End();
        };
};

class TransformLayer : public Layer
{
    public:
        enum class ButtonID
        {
            RemoveObject
        };

        enum class SliderID
        {
            PosGroup
        };

        using ButtonCallback     = std::function<void()>;
        using ButtonCallbackStr  = std::function<void(std::string)>;
        using SliderCallback     = std::function<void(float)>;
        using SliderCallbackVec3 = std::function<void(glm::vec3)>;
        using SliderCallbackPos  = std::function<void(glm::vec3, std::string)>;

        TransformLayer()
            : Layer("Transform")
        {
        }

        void OnUpdate() override {}

        static inline std::string GetSelectedObject() { return m_SelectedObject; }
        static inline void        SetSelectedObject(std::string objName) { m_SelectedObject = objName; }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }
        void SetBtnCallbackStr(ButtonID buttonID, ButtonCallbackStr callback)
        {
            m_BtnCallbacksStr[buttonID] = callback;
        }

        void SetSliderCallback(SliderID sliderID, SliderCallback callback) { m_SliderCallbacks[sliderID] = callback; }
        void SetSliderCallbackVec3(SliderID sliderID, SliderCallbackVec3 callback)
        {
            m_SliderCallbacksVec3[sliderID] = callback;
        }
        void SetSliderCallbackPos(SliderID sliderID, SliderCallbackPos callback)
        {
            m_SliderCallbacksPos[sliderID] = callback;
        }

        virtual void OnImGuiRender() override
        {
            ImVec2      btnSize  = ImVec2(150, 20);
            ImGuiStyle& style    = ImGui::GetStyle();
            style.FrameRounding  = 2;
            style.WindowRounding = 2;

            ImGui::SetNextWindowPos({300, 0}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({300, 150}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Transform");

            if (!GetSelectedObject().empty())
            {
                ImGui::SeparatorText(GetSelectedObject().c_str());

                if (ImGui::Button("Remove Object", btnSize) && m_BtnCallbacksStr[ButtonID::RemoveObject])
                {
                    m_BtnCallbacksStr[ButtonID::RemoveObject](GetSelectedObject());
                }

                ImGui::Text("Position");
                ImGui::SliderFloat("X", &m_XPos, -100.0f, 100.0f, "X = %.3f");
                ImGui::SliderFloat("Y", &m_YPos, -100.0f, 100.0f, "Y = %.3f");
                ImGui::SliderFloat("Z", &m_ZPos, -100.0f, 100.0f, "Z = %.3f");

                m_Position = {m_XPos, m_YPos, m_ZPos};

                if (m_SliderCallbacksPos[SliderID::PosGroup])
                {
                    m_SliderCallbacksPos[SliderID::PosGroup](m_Position, GetSelectedObject());
                }
            }

            ImGui::End();
        }

    private:
        float m_XPos = 0.0f, m_YPos = 0.0f, m_ZPos = 0.0f;

        glm::vec3                                        m_Position;
        std::vector<std::pair<std::string, glm::vec3>>   m_SavedPositions;
        static std::string                               m_SelectedObject;
        std::unordered_map<ButtonID, ButtonCallback>     m_BtnCallbacks;
        std::unordered_map<ButtonID, ButtonCallbackStr>  m_BtnCallbacksStr;
        std::unordered_map<SliderID, SliderCallback>     m_SliderCallbacks;
        std::unordered_map<SliderID, SliderCallbackVec3> m_SliderCallbacksVec3;
        std::unordered_map<SliderID, SliderCallbackPos>  m_SliderCallbacksPos;
};

class EntityLayer : public Layer
{
    public:
        enum class ButtonID
        {
            ClearSelection
        };

        using ButtonCallback = std::function<void()>;

        EntityLayer()
            : Layer("Entity")
        {
        }
        void OnUpdate() override {}

        void SetWindow(GLFWwindow* window) { m_Window = window; };
        void AddObjectName(std::string name) { m_ObjectNames.push_back(name); };
        void RemoveObjectName(const std::string& name)
        {
            auto it = std::find(m_ObjectNames.begin(), m_ObjectNames.end(), name);

            if (it != m_ObjectNames.end())
            {
                m_ObjectNames.erase(it);
            }
        }
        static inline void SetSelectedEntity(int selection) { m_SelectedEntity = selection; }
        static inline int  GetSelectedEntity() { return m_SelectedEntity; }
        static inline void ClearEntitySelection() { m_SelectedEntity = -1; }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }

        virtual void OnImGuiRender() override
        {
            Time& time = Time::GetInstance();

            ImVec2      btnSize  = ImVec2(150, 20);
            ImGuiStyle& style    = ImGui::GetStyle();
            style.FrameRounding  = 2;
            style.WindowRounding = 2;

            int winWidth, winHeight;
            glfwGetWindowSize(m_Window, &winWidth, &winHeight);

            ImGui::SetNextWindowPos({0, (float) winHeight - 200}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({200, 200}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Entities");

            if (ImGui::Button("Clear Selection", btnSize) && m_BtnCallbacks[ButtonID::ClearSelection])
            {
                m_BtnCallbacks[ButtonID::ClearSelection]();
            }

            int idx = 0;
            for (std::string& name : m_ObjectNames)
            {
                if (ImGui::Selectable(name.c_str(), GetSelectedEntity() == idx))
                {
                    SetSelectedEntity(idx);
                }

                ++idx;
            }

            if (m_SelectedEntity > -1)
            {
                TransformLayer::SetSelectedObject(m_ObjectNames[GetSelectedEntity()]);
            }
            else
            {
                TransformLayer::SetSelectedObject("");
            }

            ImGui::End();
        };

    private:
        static int                                   m_SelectedEntity;
        GLFWwindow*                                  m_Window;
        std::vector<std::string>                     m_ObjectNames;
        std::unordered_map<ButtonID, ButtonCallback> m_BtnCallbacks;
};

} // namespace Core

} // namespace Moonstone

#endif // BASELAYERS_H
