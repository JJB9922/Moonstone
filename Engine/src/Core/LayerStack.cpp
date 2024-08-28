#include "Include/LayerStack.h"

namespace Moonstone
{

namespace Core
{

LayerStack::LayerStack() {}

LayerStack::~LayerStack()
{
    for (Layer *layer : m_Layers)
    {
        delete layer;
    }
}

void LayerStack::PushLayer(Layer *layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
    ++m_LayerIndex;

    layer->OnAttach();
}

void LayerStack::PopLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, layer);
    if (it != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(it);
        --m_LayerIndex;
    }

    if (layer != nullptr)
    {
        delete layer;
    }
}

void LayerStack::PushOverlay(Layer *overlay) { m_Layers.emplace_back(overlay); }

void LayerStack::PopOverlay(Layer *overlay)
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
