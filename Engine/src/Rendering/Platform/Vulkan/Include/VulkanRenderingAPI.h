#ifndef VULKANrenderingAPI_H
#define VULKANrenderingAPI_H

#include "Rendering/Include/RenderingAPI.h"

namespace Moonstone
{

namespace Rendering
{

class VulkanRenderingAPI : public RenderingAPI
{
    public:
        virtual void ClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
};

} // namespace Rendering

} // namespace Moonstone

#endif // VULKANrenderingAPI_H
