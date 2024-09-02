#ifndef SHADER_H
#define SHADER_H

#include "Core/Include/Logger.h"
#include "Rendering/Include/RenderingCommand.h"
#include "mspch.h"
#include <glad/glad.h>

namespace Moonstone
{

namespace Rendering
{

class Shader
{
  public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    Shader() = default;
    void Use();

    void SetBool(const std::string &name, bool value) const;
    void SetInt(const std::string &name, int value) const;
    void SetFloat(const std::string &name, float value) const;
    void SetMat4(const std::string &name, glm::mat4 value) const;
};

} // namespace Rendering

} // namespace Moonstone

#endif // SHADER_H
