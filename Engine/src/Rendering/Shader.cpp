#include "Include/Shader.h"

namespace Moonstone
{

namespace Rendering
{

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string   vertexCode;
    std::string   fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream vShaderStream, fShaderStream;

        vShaderFile.open(vertexPath);
        vShaderStream << vShaderFile.rdbuf();
        vertexCode = vShaderStream.str();
        vShaderFile.close();

        fShaderFile.open(fragmentPath);
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        MS_ERROR("shader file not successfully read");
    }

    // m_VertexShaderCode   = vertexCode.c_str();
    // m_FragmentShaderCode = fragmentCode.c_str();

    //unsigned vertex, fragment;
    //Rendering::RenderingCommand::InitVertexShader(vertex, vShaderCode);
    //Rendering::RenderingCommand::InitFragmentShader(fragment, fShaderCode);
    //Rendering::RenderingCommand::InitShaderProgram(ID, vertex, fragment);
}

void Shader::Use() { Rendering::RenderingCommand::UseProgram(ID); }

void Shader::SetBool(const std::string& name, bool value) const
{
    Rendering::RenderingCommand::SetUniformBool(ID, name, value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    Rendering::RenderingCommand::SetUniformInt(ID, name, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    Rendering::RenderingCommand::SetUniformFloat(ID, name, value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
    Rendering::RenderingCommand::SetUniformMat4(ID, name, value);
}

} // namespace Rendering

} // namespace Moonstone
