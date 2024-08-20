#ifndef BASICSHADERS_H
#define BASICSHADERS_H

namespace Moonstone
{

namespace Renderer
{

class BasicVertexShader
{
    public:
        static inline const char* GetBasicVertexShaderSrc()
        {
            return R"(

                          #version 330 core

                            in vec2 position;
                            in vec3 color;

                            out vec3 Color;
                          
                          void main()
                          {
                              Color = color;
                              gl_Position = vec4(position, 0.0, 1.0);
                          }

                )";
        }
};

class BasicFragmentShader
{
    public:
        static inline const char* GetBasicFragmentShaderSrc()
        {
            return R"(

        #version 330 core

        in vec3 Color;

        out vec4 outColor;

        void main()
        {
            outColor = vec4(Color, 1.0f);
        }

                )";
        }
};

} // namespace Renderer

} // namespace Moonstone

#endif // BASICSHADERS_H
