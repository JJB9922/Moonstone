#include "Include/LayerStack.h"

namespace Moonstone
{

namespace Core
{

/**
 * @brief Constructs a LayerStack instance.
 *
 * Initializes an empty LayerStack.
 */
LayerStack::LayerStack() {}

/**
 * @brief Destructs a LayerStack instance.
 *
 * Deletes all layers in the stack.
 */
LayerStack::~LayerStack()
{
    for (Layer *layer : m_Layers)
    {
        delete layer;
    }
}

/**
 * @brief Adds a layer to the stack.
 *
 * Inserts the layer at the current index and increments the index. Calls
 * `OnAttach` on the layer.
 *
 * @param layer A pointer to the Layer to add.
 */
void LayerStack::PushLayer(Layer *layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
    ++m_LayerIndex;

    layer->OnAttach();
}

/**
 * @brief Removes a layer from the stack.
 *
 * Finds the layer, calls `OnDetach` on it, removes it from the stack, and
 * decrements the index.
 *
 * @param layer A pointer to the Layer to remove.
 */
void LayerStack::PopLayer(Layer *layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerIndex, layer);
    if (it != m_Layers.end())
    {
        layer->OnDetach();
        m_Layers.erase(it);
        --m_LayerIndex;
    }
}

/**
 * @brief Adds an overlay to the stack.
 *
 * Appends the overlay to the end of the stack.
 *
 * @param overlay A pointer to the Layer to add as an overlay.
 */
void LayerStack::PushOverlay(Layer *overlay) { m_Layers.emplace_back(overlay); }

/**
 * @brief Removes an overlay from the stack.
 *
 * Finds the overlay, calls `OnDetach` on it, and removes it from the stack.
 *
 * @param overlay A pointer to the Layer to remove as an overlay.
 */
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
