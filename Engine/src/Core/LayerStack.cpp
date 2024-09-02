#include "Include/LayerStack.h"

namespace Moonstone
{

namespace Core
{

LayerStack::LayerStack() {}

LayerStack::~LayerStack() {}

void LayerStack::PushLayer(std::shared_ptr<Layer> layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
    ++m_LayerIndex;

    layer->OnAttach();
}

void LayerStack::PopLayer(std::shared_ptr<Layer> layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, layer);
    if (it != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(it);
        --m_LayerIndex;
    }
}

void LayerStack::PushOverlay(std::shared_ptr<Layer> overlay) { m_Layers.emplace_back(overlay); }

void LayerStack::PopOverlay(std::shared_ptr<Layer> overlay)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, overlay);
    if (it != m_Layers.end())
    {
        overlay->OnDetach();
        m_Layers.erase(it);
    }
}

} // namespace Core

} // namespace Moonstone
