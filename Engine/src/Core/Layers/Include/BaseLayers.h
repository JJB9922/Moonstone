#ifndef BASELAYERS_H
#define BASELAYERS_H

#include "Core/Include/Layer.h"
#include "Core/Include/Time.h"
#include "Renderer/Include/Scene.h"
#include "imgui.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace Moonstone
{

namespace Core
{

class SceneLayer : public Layer
{
    public:
        SceneLayer()
            : Layer("Scene")
        {
        }

        void OnUpdate() override {}
        void SetWindow(GLFWwindow* window) { m_Window = window; }
        void SetTexMap(unsigned& texMap) { m_TexMap = texMap; }
        void SetFBParams(unsigned& FBShaderID, unsigned& screenQuadVAO, unsigned& FBOTexMap)
        {
            m_FBShaderID    = FBShaderID;
            m_ScreenQuadVAO = screenQuadVAO;
            m_FBOTexMap     = FBOTexMap;
        }

        virtual void OnImGuiRender() override

        {
            ImGui::Begin("Scene");

            auto  size      = ImGui::GetContentRegionAvail();
            float winWidth  = size.x;
            float winHeight = size.y;

            float aspectRatio = 16.0f / 9.0f;
            int   targetWidth, targetHeight;

            if (winWidth / winHeight > aspectRatio)
            {
                targetHeight = winHeight;
                targetWidth  = targetHeight * aspectRatio;
            }
            else
            {
                targetWidth  = winWidth;
                targetHeight = targetWidth / aspectRatio;
            }

            float xOffset = (winWidth - targetWidth) * 0.5f;
            float yOffset = (winHeight - targetHeight) * 0.5f;

            Renderer::RendererCommand::RescaleFramebuffer(m_TexMap, targetWidth, targetHeight);
            Renderer::RendererCommand::SetViewport(targetWidth, targetHeight);

            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 p0(pos.x + xOffset, pos.y + yOffset);
            ImVec2 p1(p0.x + targetWidth, p0.y + targetHeight);

            ImGui::GetWindowDrawList()->AddImage((void*) m_TexMap, p0, p1, ImVec2(0, 1), ImVec2(1, 0));

            Renderer::RendererCommand::DrawFrameBuffer(m_FBShaderID, m_ScreenQuadVAO, m_FBOTexMap);

            ImGui::End();
        }

    private:
        GLFWwindow* m_Window;
        unsigned    m_TexMap;
        unsigned    m_FBShaderID, m_ScreenQuadVAO, m_FBOTexMap;
};

class MenuLayer : public Layer
{
    public:
        MenuLayer()
            : Layer("Menu")
        {
        }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("New", "NS")) {}
                    if (ImGui::MenuItem("Open", "NS")) {}
                    if (ImGui::MenuItem("Save", "NS")) {}
                    if (ImGui::MenuItem("Save As...", "NS")) {}
                    if (ImGui::BeginMenu("Recent Projects"))
                    {
                        ImGui::MenuItem("placeholder");
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Edit"))
                {
                    if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                    if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
                    if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
                    if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Tools"))
                {
                    if (ImGui::MenuItem("Transform", "NS")) {}
                    if (ImGui::MenuItem("Rotate", "NS")) {}
                    if (ImGui::MenuItem("Scale", "NS")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Animation Editor", "NS")) {}
                    if (ImGui::MenuItem("Physics Editor", "NS")) {}
                    if (ImGui::MenuItem("Lighting Editor", "NS")) {}
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Play"))
                {
                    if (ImGui::MenuItem("Play", "NS")) {}
                    if (ImGui::MenuItem("Pause", "NS")) {}
                    if (ImGui::MenuItem("Stop", "NS")) {}
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Help"))
                {
                    if (ImGui::MenuItem("Documentation", "NS")) {}
                    if (ImGui::MenuItem("About", "NS")) {}
                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }
        }
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
            TransformGroup
        };

        using ButtonCallback     = std::function<void()>;
        using ButtonCallbackObj  = std::function<void(Renderer::Scene::SceneObject&)>;
        using SliderCallback     = std::function<void(float)>;
        using SliderCallbackVec3 = std::function<void(glm::vec3)>;
        using SliderCallbackObj  = std::function<void(Renderer::Scene::SceneObject&)>;

        TransformLayer()
            : Layer("Transform")
        {
        }

        void OnUpdate() override {}

        inline void SetSelectedObject(Renderer::Scene::SceneObject& obj)
        {
            m_SelectedObject = obj;

            m_XPos = m_SelectedObject.position.x;
            m_YPos = m_SelectedObject.position.y;
            m_ZPos = m_SelectedObject.position.z;

            m_XRot = m_SelectedObject.rotation.x;
            m_YRot = m_SelectedObject.rotation.y;
            m_ZRot = m_SelectedObject.rotation.z;

            m_XScale = m_SelectedObject.scale.x;
            m_YScale = m_SelectedObject.scale.y;
            m_ZScale = m_SelectedObject.scale.z;
        }

        inline void ClearSelectedObject()
        {
            m_SelectedObject.Clear();

            m_XPos = m_YPos = m_ZPos = 0.0f;
            m_XRot = m_YRot = m_ZRot = 0.0f;
            m_XScale = m_YScale = m_ZScale = 1.0f;
        }

        void SetBtnCallback(ButtonID buttonID, ButtonCallback callback) { m_BtnCallbacks[buttonID] = callback; }
        void SetBtnCallbackObj(ButtonID buttonID, ButtonCallbackObj callback)
        {
            m_BtnCallbacksObj[buttonID] = callback;
        }

        void SetSliderCallback(SliderID sliderID, SliderCallback callback) { m_SliderCallbacks[sliderID] = callback; }
        void SetSliderCallbackVec3(SliderID sliderID, SliderCallbackVec3 callback)
        {
            m_SliderCallbacksVec3[sliderID] = callback;
        }
        void SetSliderCallbackObj(SliderID sliderID, SliderCallbackObj callback)
        {
            m_SliderCallbacksObj[sliderID] = callback;
        }

        virtual void OnImGuiRender() override
        {
            ImVec2      btnSize  = ImVec2(150, 20);
            ImGuiStyle& style    = ImGui::GetStyle();
            style.FrameRounding  = 2;
            style.WindowRounding = 2;

            ImGui::SetNextWindowPos({300, 20}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({300, 150}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Transform");

            if (!m_SelectedObject.name.empty())
            {
                ImGui::SeparatorText(m_SelectedObject.name.c_str());

                if (ImGui::Button("Remove Object", btnSize) && m_BtnCallbacksObj[ButtonID::RemoveObject])
                {
                    m_BtnCallbacksObj[ButtonID::RemoveObject](m_SelectedObject);
                }

                ImGui::Text("Position");

                ImGui::Text("X: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##XPos", &m_XPos, 0.1f, -50.0f, 50.0f, "%.3f");

                ImGui::SameLine();
                ImGui::Text("Y: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##YPos", &m_YPos, 0.1f, -50.0f, 50.0f, "%.3f");

                ImGui::SameLine();
                ImGui::Text("Z: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##ZPos", &m_ZPos, 0.1f, -50.0f, 50.0f, "%.3f");

                ImGui::Text("Rotation");

                ImGui::Text("X: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##XRot", &m_XRot, 0.1f, 0.0f, 360.0f, "%.3f");

                ImGui::SameLine();
                ImGui::Text("Y: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##YRot", &m_YRot, 0.1f, 0.0f, 360.0f, "%.3f");

                ImGui::SameLine();
                ImGui::Text("Z: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);
                ImGui::DragFloat("##ZRot", &m_ZRot, 0.1f, 0.0f, 360.0f, "%.3f");

                ImGui::Text("Scale");

                ImGui::Text("X: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);

                if (ImGui::DragFloat("##XScale", &m_XScale, 0.1f, 0.0f, 20.0f, "%.3f") && m_LinkedScale)
                {
                    m_YScale = m_ZScale = m_XScale;
                }

                ImGui::SameLine();
                ImGui::Text("Y: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);

                if (ImGui::DragFloat("##YScale", &m_YScale, 0.1f, 0.0f, 20.0f, "%.3f") && m_LinkedScale)
                {
                    m_XScale = m_ZScale = m_YScale;
                }

                ImGui::SameLine();
                ImGui::Text("Z: ");
                ImGui::SameLine();
                ImGui::PushItemWidth(50);

                if (ImGui::DragFloat("##ZScale", &m_ZScale, 0.1f, 0.0f, 20.0f, "%.3f") && m_LinkedScale)
                {
                    m_XScale = m_YScale = m_ZScale;
                }

                ImGui::SameLine();
                if (ImGui::Checkbox("Link", &m_LinkedScale))
                {
                    if (m_LinkedScale)
                    {
                        m_YScale = m_ZScale = m_XScale;
                    }
                }

                m_SelectedObject.position = {m_XPos, m_YPos, m_ZPos};
                m_SelectedObject.rotation = {m_XRot, m_YRot, m_ZRot};
                m_SelectedObject.scale    = {m_XScale, m_YScale, m_ZScale};

                if (m_SliderCallbacksObj[SliderID::TransformGroup])
                {
                    m_SliderCallbacksObj[SliderID::TransformGroup](m_SelectedObject);
                }
            }

            ImGui::End();
        }

    private:
        float m_XPos, m_YPos, m_ZPos;
        float m_XRot, m_YRot, m_ZRot;
        float m_XScale = 1.0f, m_YScale = 1.0f, m_ZScale = 1.0f;
        bool  m_LinkedScale;

        Renderer::Scene::SceneObject                     m_SelectedObject;
        std::unordered_map<ButtonID, ButtonCallback>     m_BtnCallbacks;
        std::unordered_map<ButtonID, ButtonCallbackObj>  m_BtnCallbacksObj;
        std::unordered_map<SliderID, SliderCallback>     m_SliderCallbacks;
        std::unordered_map<SliderID, SliderCallbackVec3> m_SliderCallbacksVec3;
        std::unordered_map<SliderID, SliderCallbackObj>  m_SliderCallbacksObj;
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
        void AddObject(Renderer::Scene::SceneObject& object) { m_Objects.push_back(object); };
        void RemoveObject(const Renderer::Scene::SceneObject& object)
        {
            auto it = std::find_if(m_Objects.begin(),
                                   m_Objects.end(),
                                   [&object](const Renderer::Scene::SceneObject& obj)
                                   { return obj.name == object.name; });

            if (it != m_Objects.end())
            {
                m_Objects.erase(it);
            }
        }
        void SetObjectVector(std::vector<Renderer::Scene::SceneObject> objects) { m_Objects = objects; }

        void OverwriteObject(Renderer::Scene::SceneObject& object) {}

        static inline void SetSelectedEntity(int selection) { m_SelectedEntity = selection; }
        static inline int  GetSelectedEntity() { return m_SelectedEntity; }
        static inline void ClearEntitySelection() { m_SelectedEntity = -1; }

        inline std::vector<Renderer::Scene::SceneObject> GetObjects() { return GetObjects(); }

        void SetTransformLayer(TransformLayer* transformLayer) { m_TransformLayer = transformLayer; }

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

            // Raw loop :/
            for (int i = 0; i < m_Objects.size(); ++i)
            {
                if (ImGui::Selectable(m_Objects[i].name.c_str(), GetSelectedEntity() == i))
                {
                    SetSelectedEntity(i);
                    if (m_TransformLayer)
                    {
                        m_TransformLayer->SetSelectedObject(m_Objects[i]);
                    }
                }
            }

            if (GetSelectedEntity() == -1 && m_TransformLayer)
            {
                m_TransformLayer->ClearSelectedObject();
            }

            ImGui::End();
        };

    private:
        static int                                   m_SelectedEntity;
        GLFWwindow*                                  m_Window;
        std::vector<Renderer::Scene::SceneObject>    m_Objects;
        std::unordered_map<ButtonID, ButtonCallback> m_BtnCallbacks;
        TransformLayer*                              m_TransformLayer;
};

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

            ImGui::SetNextWindowPos({0, 20}, ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize({300, 100}, ImGuiCond_FirstUseEver);

            ImGui::Begin("Debug");
            float fps = 1.0f / time.GetDeltaTime();

            ImGui::Text("FPS: %.2f", fps);
            ImGui::Text("Delta Time: %.4f seconds", time.GetDeltaTime());

            ImGui::End();
        };
};

} // namespace Core

} // namespace Moonstone

#endif // BASELAYERS_H
