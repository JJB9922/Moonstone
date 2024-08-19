#ifndef BASELAYERS_H
#define BASELAYERS_H

#include "Core/Include/Layer.h"
#include "imgui.h"

namespace Moonstone
{

namespace Core
{

class ExampleLayer : public Layer
{
    public:
        ExampleLayer()
            : Layer("Example")
        {
        }

        void OnUpdate() override {}

        virtual void OnImGuiRender() override
        {
            ImGui::Begin("Moonstone");
            ImGui::Text("Moonstone");
            ImGui::End();
        };
};

} // namespace Core

} // namespace Moonstone

#endif // BASELAYERS_H
