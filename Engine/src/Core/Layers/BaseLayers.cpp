#include "Core/Layers/Include/BaseLayers.h"

namespace Moonstone
{

namespace Core
{

int EntityLayer::m_SelectedEntity = -1;

// huh wuh
Renderer::Scene::SceneObject  nullObj;
Renderer::Scene::SceneObject& TransformLayer::m_SelectedObject = nullObj;

} // namespace Core

} // namespace Moonstone
