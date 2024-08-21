#include "Include/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Moonstone
{

namespace Renderer
{

/**
 * @brief Sets the clear color for the OpenGL context.
 *
 * Configures the color that will be used when clearing the color buffer.
 *
 * @param color A glm::vec4 representing the RGBA color to set.
 */
void OpenGLRendererAPI::ClearColor(const glm::vec4 &color) { glClearColor(color.r, color.g, color.b, color.a); }

/**
 * @brief Clears the color and depth buffers.
 *
 * This function clears the color and depth buffers to the values set by
 * `ClearColor`. It prepares the screen for the next rendering pass.
 */
void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

/**
 * @brief Initializes a vertex shader.
 *
 * Compiles the provided vertex shader source code and stores the shader ID
 * in the provided reference.
 *
 * @param vertexShader Reference to an unsigned integer where the shader ID will be stored.
 * @param vertexShaderSrc A pointer to the source code of the vertex shader.
 */
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

/**
 * @brief Initializes a fragment shader.
 *
 * Compiles the provided fragment shader source code and stores the shader ID
 * in the provided reference.
 *
 * @param fragmentShader Reference to an unsigned integer where the shader ID will be stored.
 * @param fragmentShaderSrc A pointer to the source code of the fragment shader.
 */
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

/**
 * @brief Initializes a shader program.
 *
 * Creates a shader program, attaches the provided vertex and fragment shaders,
 * and links the program. The shader program ID is stored in the provided reference.
 *
 * @param shaderProgram Reference to an unsigned integer where the shader program ID will be stored.
 * @param vertexShader The vertex shader ID to attach.
 * @param fragmentShader The fragment shader ID to attach.
 */
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

/**
 * @brief Initializes a vertex array object (VAO).
 *
 * Generates and binds a VAO, preparing it for use in rendering.
 *
 * @param VAO Reference to an unsigned integer where the VAO ID will be stored.
 */
void OpenGLRendererAPI::InitVertexArray(unsigned &VAO)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
};

/**
 * @brief Initializes a vertex buffer object (VBO).
 *
 * Generates and binds a VBO, and uploads vertex data to the buffer.
 *
 * @param VBO Reference to an unsigned integer where the VBO ID will be stored.
 * @param vertices Pointer to the vertex data to upload.
 * @param size Size of the vertex data in bytes.
 */
void OpenGLRendererAPI::InitVertexBuffer(unsigned &VBO, float *vertices, size_t size)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
};

/**
 * @brief Initializes an element buffer object (EBO).
 *
 * Generates and binds an EBO, and uploads index data to the buffer.
 *
 * @param EBO Reference to an unsigned integer where the EBO ID will be stored.
 * @param indices Pointer to the index data to upload.
 * @param size Size of the index data in bytes.
 */
void OpenGLRendererAPI::InitElementBuffer(unsigned &EBO, unsigned *indices, size_t size)
{
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
};

/**
 * @brief Initializes vertex attributes for the currently bound VAO.
 *
 * Configures the vertex attribute pointers for the vertex data layout.
 */
void OpenGLRendererAPI::InitVertexAttributes()
{
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
};

/**
 * @brief Submits draw commands for rendering.
 *
 * Uses the specified shader program and VAO to render the geometry.
 *
 * @param shaderProgram The shader program ID to use for rendering.
 * @param VAO The VAO ID that contains the vertex data to render.
 */
void OpenGLRendererAPI::SubmitDrawCommands(unsigned shaderProgram, unsigned VAO)
{
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
};

/**
 * @brief Sets the polygon mode for rasterization.
 *
 * Configures whether polygons are rendered as lines or filled shapes.
 *
 * @param polygonMode The mode to set for rendering polygons, defined by DataType.
 */
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

/**
 * @brief Cleans up OpenGL resources.
 *
 * Deletes the VAO, VBO, and shader program to free up resources.
 *
 * @param VAO The VAO ID to delete.
 * @param VBO The VBO ID to delete.
 * @param shaderProgram The shader program ID to delete.
 */
void OpenGLRendererAPI::Cleanup(unsigned &VAO, unsigned &VBO, unsigned &shaderProgram)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}

} // namespace Renderer

} // namespace Moonstone
