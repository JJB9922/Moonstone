#ifndef SHADER_H
#define SHADER_H

#include "Core/Include/Logger.h"
#include "Renderer/Include/RendererCommand.h"
#include "mspch.h"
#include <glad/glad.h>

namespace Moonstone
{

namespace Renderer
{

class Shader
{
    public:
        unsigned int ID;

        Shader(const char *vertexPath, const char *fragmentPath);
        void Use();

        void SetBool(const std::string &name, bool value) const;
        void SetInt(const std::string &name, int value) const;
        void SetFloat(const std::string &name, float value) const;
        void SetMat4(const std::string &name, glm::mat4 value) const;
};

} // namespace Renderer

} // namespace Moonstone

#endif // SHADER_H
