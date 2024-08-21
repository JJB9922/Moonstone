#ifndef BASICSHADERS_H
#define BASICSHADERS_H

namespace Moonstone
{

namespace Renderer
{

class BasicVertexShader
{
    public:
        /**
        * @brief Provides the source code for a basic vertex shader.
        *
        * @return A string literal containing the GLSL source code for the vertex shader.
        */
        static inline const char* GetBasicVertexShaderSrc()
        {
            return R"(

                          #version 330 core

                            layout(location = 0) in vec3 aPos;
                          
                          void main()
                          {
                              gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
                          }

                )";
        }
};

/**
 * @brief Provides the source code for a basic fragment shader.
 *
 * @return A string literal containing the GLSL source code for the fragment shader.
 */
class BasicFragmentShader
{
    public:
        static inline const char* GetBasicFragmentShaderSrc()
        {
            return R"(

        #version 330 core

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
        }

                )";
        }
};

} // namespace Renderer

} // namespace Moonstone

#endif // BASICSHADERS_H
