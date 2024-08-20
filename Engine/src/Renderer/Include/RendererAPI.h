#ifndef RENDERERAPI_H
#define RENDERERAPI_H

namespace Moonstone
{

namespace Renderer
{

class RendererAPI
{
    public:
        enum class API
        {
            None,
            OpenGL,
            Vulkan
        };

    public:
        inline static API GetAPI() { return s_API; }

    private:
        static API s_API;
};

} // namespace Renderer

} // namespace Moonstone

#endif // RENDERERAPI_H
