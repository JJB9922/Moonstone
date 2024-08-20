#ifndef VULKANRENDERERAPI_H
#define VULKANRENDERERAPI_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class VulkanRendererAPI : public RendererAPI
{
    public:
        virtual void ClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
};

} // namespace Renderer

} // namespace Moonstone

#endif // VULKANRENDERERAPI_H
