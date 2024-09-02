#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Core/Include/Layer.h"

namespace Moonstone
{

namespace Core
{

class LayerStack
{
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(std::shared_ptr<Layer> layer);
        void PopLayer(std::shared_ptr<Layer> layer);

        void PushOverlay(std::shared_ptr<Layer> overlay);
        void PopOverlay(std::shared_ptr<Layer> overlay);

        std::vector<std::shared_ptr<Layer>>::iterator begin() { return m_Layers.begin(); }
        std::vector<std::shared_ptr<Layer>>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<std::shared_ptr<Layer>> m_Layers;
        unsigned             m_LayerIndex = 0;
};

} // namespace Core

} // namespace Moonstone

#endif // LAYERSTACK_H
