#include "Include/EditorUI.h"
#include "Include/Logger.h"
#include "Layers/Include/BaseLayers.h"
#include "Rendering/Include/Lighting.h"
#include "Rendering/Include/Renderer.h"
#include "Rendering/Include/SceneManager.h"
#include <memory>
#include <sstream>
#include <string>

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

    auto transformLayer = std::make_shared<TransformLayer>();
    auto entityLayer = std::make_shared<EntityLayer>();
    entityLayer->SetWindow(m_Window->m_Window);
    entityLayer->SetBtnCallback(EntityLayer::ButtonID::ClearSelection, [this, entityLayer]() {
        entityLayer->ClearObjectEntitySelection();
        entityLayer->ClearLightEntitySelection();
    });

    entityLayer->SetTransformLayer(transformLayer);
    PushLayer(entityLayer);

    transformLayer->SetBtnCallbackObj(
        TransformLayer::ButtonID::RemoveObject, [this, entityLayer](Rendering::SceneObject &object) {
            auto it = std::find_if(m_ActiveScene->objects.begin(), m_ActiveScene->objects.end(),
                                   [&object](Rendering::SceneObject &obj) { return obj.name == object.name; });

            if (it != m_ActiveScene->objects.end())
            {
                m_ActiveScene->objects.erase(it);
            }

            entityLayer->ClearObjectEntitySelection();
            entityLayer->RemoveObject(object);
        });

    transformLayer->SetBtnCallbackLight(
        TransformLayer::ButtonID::RemoveLight, [this, entityLayer](Rendering::Lighting::Light &light) {
            auto it = std::find_if(m_ActiveScene->lights.begin(), m_ActiveScene->lights.end(),
                                   [&light](Rendering::Lighting::Light &lt) { return lt.id == light.id; });

            if (it != m_ActiveScene->lights.end())
            {
                Rendering::Renderer renderer(m_ActiveScene);

                if (light.type == Rendering::Lighting::LightType::Directional)
                {
                    renderer.DeactivateDirectionalLight();
                }
                else if (light.type == Rendering::Lighting::LightType::Point)
                {
                    renderer.DeactivatePointLight(light);
                }

                m_ActiveScene->lights.erase(it);
            }

            entityLayer->ClearLightEntitySelection();
            entityLayer->RemoveLight(light);
        });

    transformLayer->SetSliderCallbackObj(
        TransformLayer::SliderID::ObjectTransformGroup, [this, entityLayer](Rendering::SceneObject &object) {
            auto it = std::find_if(m_ActiveScene->objects.begin(), m_ActiveScene->objects.end(),
                                   [&object](Rendering::SceneObject &obj) { return obj.name == object.name; });

            if (it != m_ActiveScene->objects.end())
            {
                it->position = object.position;
                it->rotation = object.rotation;
                it->scale = object.scale;
            }

            entityLayer->SetObjectVector(m_ActiveScene->objects);
        });

    transformLayer->SetSliderCallbackLight(
        TransformLayer::SliderID::LightTransformGroup, [this, entityLayer](Rendering::Lighting::Light &light) {
            auto it = std::find_if(m_ActiveScene->lights.begin(), m_ActiveScene->lights.end(),
                                   [&light](Rendering::Lighting::Light &lt) { return lt.id == light.id; });

            if (it != m_ActiveScene->lights.end())
            {
                it->id = light.id;
                it->position = light.position;
            }

            entityLayer->SetLightVector(m_ActiveScene->lights);
        });

    PushLayer(transformLayer);

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

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddObject,
                                  [this, controlsLayer, entityLayer, transformLayer]() {
                                      ControlsLayer::SceneObject obj = controlsLayer->GetAddObject();

                                      Rendering::SceneManager sceneManager;
                                      sceneManager.AddModelToScene(m_ActiveScene);
                                      entityLayer->AddObject(m_ActiveScene->objects.back());

                                      entityLayer->ClearObjectEntitySelection();
                                      entityLayer->ClearLightEntitySelection();

                                      entityLayer->SetSelectedObjectEntity(m_ActiveScene->objects.size() - 1);
                                      transformLayer->SetSelectedObject(m_ActiveScene->objects.back());
                                  });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddDirectionalLight,
                                  [this, controlsLayer, entityLayer, transformLayer]()
                                  {
                                      bool dirLightExists = false;

                                      for (auto &light : m_ActiveScene->lights)
                                      {
                                          if (light.type == Rendering::Lighting::LightType::Directional)
                                          {
                                              dirLightExists = true;
                                          }
                                      }

                                      if (dirLightExists == true)
                                      {
                                          MS_INFO("cannot add directional light as one already exists");
                                          return;
                                      }

                                      Rendering::SceneManager sceneManager;

                                      // TODO not string...
                                      std::stringstream ss;
                                      ss << "DirLight_" << m_ActiveScene->lights.size();

                                      auto dirLight = Rendering::Lighting::Light(ss.str(),
                                                                                 {0.5f, -1.0f, 0.5f},
                                                                                 {0.1f, 0.1f, 0.1f},
                                                                                 {0.5f, 0.5f, 0.5f},
                                                                                 {0.2f, 0.2f, 0.2f},
                                                                                 true);

                                      sceneManager.AddLightToScene(m_ActiveScene, dirLight);
                                      entityLayer->AddLight(m_ActiveScene->lights.back());

                                      entityLayer->ClearObjectEntitySelection();
                                      entityLayer->ClearLightEntitySelection();

                                      entityLayer->SetSelectedLightEntity(m_ActiveScene->lights.size() - 1);
                                      transformLayer->SetSelectedLight(m_ActiveScene->lights.back());
                                  });

    controlsLayer->SetSliderCallback(ControlsLayer::SliderID::DirectionalLightAngle,
                                     [this, controlsLayer](float dirLightFloat)
                                     {
                                         float sliderFloat = dirLightFloat;

                                         float angle = sliderFloat * 2.0f * 3.14159f;

                                         float y = cos(angle);
                                         float x = sin(angle);
                                         float z = 0.0f;

                                         glm::vec3 lightDirection = normalize(glm::vec3(x, y, z));

                                         for (auto &light : m_ActiveScene->lights)
                                         {
                                             if (light.type == Rendering::Lighting::LightType::Directional)
                                             {
                                                 light.direction = lightDirection;

                                                 break;
                                             }
                                         }
                                     });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::AddPointLight,
                                  [this, controlsLayer, entityLayer, transformLayer]()
                                  {
                                      Rendering::SceneManager sceneManager;

                                      std::stringstream ss;
                                      ss << "PointLight_" << m_ActiveScene->lights.size();

                                      // TODO do some serious stuff w this not nonense
                                      auto ptLight = Rendering::Lighting::Light(ss.str(),
                                                                                {0.5f, 0.5f, 0.5f},
                                                                                {0.1f, 0.1f, 0.1f},
                                                                                {0.5f, 0.5f, 0.5f},
                                                                                {0.2f, 0.2f, 0.2f},
                                                                                true,
                                                                                1.0f,
                                                                                0.09f,
                                                                                0.0032f);

                                      sceneManager.AddLightToScene(m_ActiveScene, ptLight);
                                      entityLayer->AddLight(m_ActiveScene->lights.back());

                                      entityLayer->ClearObjectEntitySelection();
                                      entityLayer->ClearLightEntitySelection();

                                      entityLayer->SetSelectedLightEntity(m_ActiveScene->lights.size() - 1);
                                      transformLayer->SetSelectedLight(m_ActiveScene->lights.back());
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
