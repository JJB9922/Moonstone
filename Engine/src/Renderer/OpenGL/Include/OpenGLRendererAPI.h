#ifndef OPENGLRENDERERAPI_H
#define OPENGLRENDERERAPI_H

#include "Renderer/Include/RendererAPI.h"

namespace Moonstone
{

namespace Renderer
{

class OpenGLRendererAPI : public RendererAPI
{
    public:
        virtual void ClearColor(const glm::vec4& color) override;
        virtual void Clear() override;
};

} // namespace Renderer

} // namespace Moonstone
#endif // OPENGLRENDERERAPI_H
