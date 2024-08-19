#ifndef LAYER_H
#define LAYER_H

#include "Core/Events/Include/Event.h"
#include "Core/Include/Core.h"

namespace Moonstone
{

namespace Core
{

class Layer
{
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}

        virtual void OnEvent(Event &event) {}

        inline const std::string &GetLayerName() const { return m_LayerName; }

    private:
        std::string m_LayerName;
};

} // namespace Core

} // namespace Moonstone

#endif // LAYER_H
