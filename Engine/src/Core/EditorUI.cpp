#include "Include/EditorUI.h"
#include "Layers/Include/BaseLayers.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/SceneManager.h"
#include <memory>
#include <sstream>

namespace Moonstone
{

namespace Core
{

EditorUI::~EditorUI()
{
    Shutdown();
}

void EditorUI::Init()
{
    m_ImGuiLayer = std::make_shared<Tools::ImGuiLayer>();
    m_ImGuiLayer->SetWindow(m_Window->m_Window);
    PushOverlay(m_ImGuiLayer);

    auto sceneLayer = std::make_shared<SceneLayer>();
    sceneLayer->SetWindow(m_Window->m_Window);
    sceneLayer->SetTexMap(m_FBOTextureMap);
    sceneLayer->SetFBParams(m_FBShaderID, m_ScreenQuadVAO, m_FBOTextureMap);
    PushLayer(sceneLayer);

    auto menuLayer = std::make_shared<MenuLayer>();
    PushLayer(menuLayer);

    auto debugLayer = std::make_shared<DebugLayer>();
    PushLayer(debugLayer);

    // auto transformLayer = std::make_shared<TransformLayer>();
    auto entityLayer = std::make_shared<EntityLayer>();
    entityLayer->SetWindow(m_Window->m_Window);
    entityLayer->SetBtnCallback(EntityLayer::ButtonID::ClearSelection,
                                [this, entityLayer]() { entityLayer->ClearEntitySelection(); });
    // entityLayer->SetTransformLayer(transformLayer);
    PushLayer(entityLayer);

    /*
        transformLayer->SetBtnCallbackObj(
            TransformLayer::ButtonID::RemoveObject, [this, entityLayer](Rendering::SceneObject &object) {
            auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                                   [&object](Rendering::SceneObject &obj) { return obj.name == object.name; });

            if (it != m_Objects.end())
            {
                m_Objects.erase(it);
            }

            entityLayer->ClearEntitySelection();
            entityLayer->RemoveObject(object);
            });

        transformLayer->SetSliderCallbackObj(
            TransformLayer::SliderID::TransformGroup, [this, entityLayer](Rendering::SceneObject &object) {
            auto it = std::find_if(m_Objects.begin(), m_Objects.end(),
                                   [&object](Rendering::SceneObject &obj) { return obj.name == object.name; });

            if (it != m_Objects.end())
            {
                it->position = object.position;
                it->rotation = object.rotation;
                it->scale = object.scale;
            }

            entityLayer->SetObjectVector(m_Objects);
            });
        PushLayer(transformLayer);
    */
    auto controlsLayer = std::make_shared<ControlsLayer>();
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::Exit, [this]() { m_Window->TerminateWindow(); });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyBGColor, [this, controlsLayer]() {
        auto color = controlsLayer->GetBGColor();
        m_ActiveScene->background.r = color.x;
        m_ActiveScene->background.g = color.y;
        m_ActiveScene->background.b = color.z;
        m_ActiveScene->background.a = color.w;
    });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleWireframe, [this]() {
        if (m_Window->m_PolygonMode == Rendering::RenderingAPI::PolygonDataType::PolygonLine)
        {
            Rendering::RenderingCommand::SetPolygonMode(Rendering::RenderingAPI::PolygonDataType::PolygonFill);

            m_Window->m_PolygonMode = Rendering::RenderingAPI::PolygonDataType::PolygonFill;
        }
        else
        {
            Rendering::RenderingCommand::SetPolygonMode(Rendering::RenderingAPI::PolygonDataType::PolygonLine);

            m_Window->m_PolygonMode = Rendering::RenderingAPI::PolygonDataType::PolygonLine;
        }
    });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyCameraSens, [this, controlsLayer]() {
        m_Window->SetCameraSens(controlsLayer->GetCamSensitivity());
    });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleGrid, [this]() {
        {
            m_ActiveScene->isGridEnabled = !m_ActiveScene->isGridEnabled;
        }
    });
    /*
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddObject,
                                  [this, controlsLayer, entityLayer, transformLayer]()
                                  {
                                      ControlsLayer::SceneObject obj = controlsLayer->GetAddObject();

                                      switch (obj)
                                      {
                                          case ControlsLayer::SceneObject::Cube:
                                              //AddCube();
                                              //entityLayer->AddObject(m_Objects.back());
                                              entityLayer->ClearEntitySelection();
                                              //entityLayer->SetSelectedEntity(m_Objects.size() - 1);
                                              //transformLayer->SetSelectedObject(m_Objects.back());
                                              break;
                                      }
                                  });
 */
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddDirectionalLight, [this, controlsLayer, entityLayer]() {
        // TODO not string...
        std::stringstream ss;
        ss << "DirLight_" << m_ActiveScene->lighting.GetLights().size();

        auto dirLight = Rendering::Lighting::Light(ss.str(), {0.5f, -1.0f, 0.5f}, {0.1f, 0.1f, 0.1f},
                                                   {0.8f, 0.8f, 0.8f}, {0.5f, 0.5f, 0.5f}, true);
        Rendering::SceneManager sceneManager;
        sceneManager.AddLightToScene(m_ActiveScene, dirLight);
    });

    controlsLayer->SetSliderCallback(ControlsLayer::SliderID::DirectionalLightAngle,
                                     [this, controlsLayer](float dirLightFloat) {
                                         float sliderFloat = dirLightFloat;

                                         float angle = sliderFloat * 2.0f * 3.14159f;

                                         float y = cos(angle);
                                         float x = sin(angle);
                                         float z = 0.0f;

                                         glm::vec3 lightDirection = normalize(glm::vec3(x, y, z));

                                         for (auto light : m_ActiveScene->lighting.GetLights())
                                         {
                                             if (light.type == Rendering::Lighting::LightType::Directional)
                                             {
                                                 light.direction = lightDirection;
                                                 m_ActiveScene->lighting.SetLight(light.id, light);
                                             }
                                         }
                                     });

    m_Layers.push_back(*controlsLayer);
    PushLayer(controlsLayer);
}

void EditorUI::Shutdown()
{
    // Clean up ImGui resources here
}

void EditorUI::Render()
{
    for (auto layer : m_LayerStack)
    {
        layer->OnUpdate();
    }

    m_ImGuiLayer->Start();
    for (auto layer : m_LayerStack)
    {
        layer->OnImGuiRender();
    }
    m_ImGuiLayer->End();
}

void EditorUI::PushLayer(std::shared_ptr<Layer> layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void EditorUI::PopLayer(std::shared_ptr<Layer> layer)
{
    m_LayerStack.PopLayer(layer);
}

void EditorUI::PushOverlay(std::shared_ptr<Layer> layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void EditorUI::PopOverlay(std::shared_ptr<Layer> overlay)
{
    m_LayerStack.PopOverlay(overlay);
}

} // namespace Core

} // namespace Moonstone
