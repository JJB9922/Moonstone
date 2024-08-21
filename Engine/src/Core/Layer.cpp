#include "Include/Layer.h"

namespace Moonstone
{

namespace Core
{

/**
 * @brief Constructs a Layer instance.
 *
 * Initializes the layer with the specified name.
 *
 * @param layerName The name of the layer.
 */
Layer::Layer(const std::string &layerName)
    : m_LayerName(layerName)
{
}

} // namespace Core

} // namespace Moonstone
