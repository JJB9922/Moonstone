#include "Include/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Moonstone
{

namespace Renderer
{

void OpenGLRendererAPI::ClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::InitVertexShader(unsigned &vertexShader, const char *vertexShaderSrc)
{
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        MS_ERROR("vertex shader failed to compile: {0}", infoLog);
        return;
    }
}

void OpenGLRendererAPI::InitFragmentShader(unsigned &fragmentShader, const char *fragmentShaderSrc)
{
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
    glCompileShader(fragmentShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        MS_ERROR("fragment shader failed to compile: {0}", infoLog);
        return;
    }
}

void OpenGLRendererAPI::InitShaderProgram(unsigned &shaderProgram, unsigned &vertexShader, unsigned &fragmentShader)
{
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int  success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        MS_ERROR("shader program failed to link: {0}", infoLog);
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void OpenGLRendererAPI::InitVertexArray(unsigned &VAO)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
};

void OpenGLRendererAPI::InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

void OpenGLRendererAPI::InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
};

void OpenGLRendererAPI::InitVertexAttributes()
{
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
};

void OpenGLRendererAPI::SubmitDrawCommands(unsigned shaderProgram, unsigned VAO)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
};

void OpenGLRendererAPI::SetPolygonMode(DataType polygonMode)
{
    switch (polygonMode)
    {
        case DataType::PolygonLine:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case DataType::PolygonFill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
};

void OpenGLRendererAPI::Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

} // namespace Renderer

} // namespace Moonstone
