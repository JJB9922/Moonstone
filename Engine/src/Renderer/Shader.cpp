#include "Include/Shader.h"

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Retrieve shaders from file
 * @param vertexPath
 * @param fragmentPath
 */
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

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned vertex, fragment;
    Renderer::RendererCommand::InitVertexShader(vertex, vShaderCode);
    Renderer::RendererCommand::InitFragmentShader(fragment, fShaderCode);
    Renderer::RendererCommand::InitShaderProgram(ID, vertex, fragment);
}

/**
 * @brief Use the current shader program
 */
void Shader::Use() { Renderer::RendererCommand::UseProgram(ID); }

void Shader::SetBool(const std::string& name, bool value) const
{
    Renderer::RendererCommand::SetUniformBool(ID, name, value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    Renderer::RendererCommand::SetUniformInt(ID, name, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    Renderer::RendererCommand::SetUniformFloat(ID, name, value);
}

} // namespace Renderer

} // namespace Moonstone
