#ifndef IMGUILAYER_H
#define IMGUILAYER_H

#include "Core/Include/Layer.h"

namespace Moonstone
{

namespace Tools
{

class ImGuiLayer : public Core::Layer
{
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Start();
        void End();

    private:
        float m_Time = 0.0f;
};

} // namespace Tools

} // namespace Moonstone

#endif // IMGUILAYER_H
