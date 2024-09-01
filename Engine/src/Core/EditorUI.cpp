#include "Include/EditorUI.h"

namespace Moonstone
{

namespace Core
{

EditorUI::~EditorUI() { Shutdown(); }

void EditorUI::Init()
{
    // auto sceneLayer = new SceneLayer;
    // sceneLayer->SetWindow(m_Window.m_Window);
    //   sceneLayer->SetTexMap(m_FBOTextureMap);
    //  sceneLayer->SetFBParams(m_FBShaderID, m_ScreenQuadVAO, m_FBOTextureMap);
    // PushLayer(sceneLayer);

    auto menuLayer = new MenuLayer;
    PushLayer(menuLayer);

    auto debugLayer = new DebugLayer;
    PushLayer(debugLayer);

    auto transformLayer = new TransformLayer();
    auto entityLayer    = new EntityLayer;
    entityLayer->SetWindow(m_Window->m_Window);
    entityLayer->SetBtnCallback(EntityLayer::ButtonID::ClearSelection,
                                [this, entityLayer]() { entityLayer->ClearEntitySelection(); });
    entityLayer->SetTransformLayer(transformLayer);
    PushLayer(entityLayer);

    transformLayer->SetBtnCallbackObj(TransformLayer::ButtonID::RemoveObject,
                                      [this, entityLayer](Rendering::SceneObject& object)
                                      {
                                          /*
                                            
                                          auto it = std::find_if(m_Objects.begin(),
                                                                 m_Objects.end(),
                                                                 [&object](Rendering::SceneObject& obj)
                                                                 { return obj.name == object.name; });

                                          if (it != m_Objects.end())
                                          {
                                              m_Objects.erase(it);
                                          }

                                          entityLayer->ClearEntitySelection();
                                          entityLayer->RemoveObject(object);
*/
                                      });

    transformLayer->SetSliderCallbackObj(TransformLayer::SliderID::TransformGroup,
                                         [this, entityLayer](Rendering::SceneObject& object)
                                         {
                                             /*
                                             auto it = std::find_if(m_Objects.begin(),
                                                                    m_Objects.end(),
                                                                    [&object](Rendering::SceneObject& obj)
                                                                    { return obj.name == object.name; });

                                             if (it != m_Objects.end())
                                             {
                                                 it->position = object.position;
                                                 it->rotation = object.rotation;
                                                 it->scale    = object.scale;
                                             }

                                             entityLayer->SetObjectVector(m_Objects);*/
                                         });
    PushLayer(transformLayer);

    auto controlsLayer = new ControlsLayer;
    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::Exit, [this]() { m_Window->TerminateWindow(); });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyBGColor,
                                  [this, controlsLayer]()
                                  {
                                      auto color                = controlsLayer->GetBGColor();
                                      m_Window->m_WindowColor.r = color.x;
                                      m_Window->m_WindowColor.g = color.y;
                                      m_Window->m_WindowColor.b = color.z;
                                      m_Window->m_WindowColor.a = color.w;
                                  });

    controlsLayer
        ->SetBtnCallback(ControlsLayer::ButtonID::ToggleWireframe,
                         [this]()
                         {
                             if (m_Window->m_PolygonMode == Rendering::RenderingAPI::PolygonDataType::PolygonLine)
                             {
                                 Rendering::RenderingCommand::SetPolygonMode(
                                     Rendering::RenderingAPI::PolygonDataType::PolygonFill);

                                 m_Window->m_PolygonMode = Rendering::RenderingAPI::PolygonDataType::PolygonFill;
                             }
                             else
                             {
                                 Rendering::RenderingCommand::SetPolygonMode(
                                     Rendering::RenderingAPI::PolygonDataType::PolygonLine);

                                 m_Window->m_PolygonMode = Rendering::RenderingAPI::PolygonDataType::PolygonLine;
                             }
                         });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ApplyCameraSens,
                                  [this, controlsLayer]()
                                  { m_Window->SetCameraSens(controlsLayer->GetCamSensitivity()); });

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleGrid,
                                  [this]()
                                  {
                                      {
                                          //m_DefaultGrid = !m_DefaultGrid;
                                      }
                                  });

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

    controlsLayer->SetBtnCallback(ControlsLayer::ButtonID::ToggleSunlight,
                                  [this, controlsLayer, entityLayer]() { //ToggleSunlight();
                                  });

    controlsLayer->SetSliderCallback(ControlsLayer::SliderID::TimeOfDay,
                                     [this, controlsLayer](float timeOfDayFloat)
                                     {
                                         float timeOfDay = timeOfDayFloat;

                                         float angle = timeOfDay * 2.0f * 3.14159f;

                                         float y = cos(angle);
                                         float x = sin(angle);
                                         float z = 0.0f;

                                         glm::vec3 lightDirection = normalize(glm::vec3(x, y, z));

                                         //m_TimeOfDay = lightDirection;
                                     });

    m_Layers.push_back(*controlsLayer);
    PushLayer(controlsLayer);
}

void EditorUI::Shutdown()
{
    // Clean up ImGui resources here
}

void EditorUI::PushLayer(Layer* layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void EditorUI::PopLayer(Layer* layer) { m_LayerStack.PopLayer(layer); }

void EditorUI::PushOverlay(Layer* layer)
{
    m_LayerStack.PushOverlay(layer);
    layer->OnAttach();
}

void EditorUI::PopOverlay(Layer* overlay) { m_LayerStack.PopOverlay(overlay); }

} // namespace Core

} // namespace Moonstone
