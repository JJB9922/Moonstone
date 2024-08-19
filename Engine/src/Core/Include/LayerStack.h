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

        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);

        void PushOverlay(Layer *overlay);
        void PopOverlay(Layer *overlay);

        std::vector<Layer *>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer *>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer *> m_Layers;
        unsigned             m_LayerIndex = 0;
};

} // namespace Core

} // namespace Moonstone

#endif // LAYERSTACK_H
