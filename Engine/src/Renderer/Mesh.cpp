#include "Include/Mesh.h"

namespace Moonstone
{

namespace Renderer
{

void Mesh::SetupMesh()
{
    Renderer::RendererCommand::InitVertexArray(m_VAO);
    Renderer::RendererCommand::InitVertexBuffer(m_VBO,
                                                reinterpret_cast<float*>(vertices.data()),
                                                vertices.size() * sizeof(Vertex));
    Renderer::RendererCommand::InitElementBuffer(m_EBO, indices.data(), indices.size() * sizeof(unsigned));

    Renderer::RendererCommand::InitVertexAttributes(0,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    0);
    Renderer::RendererCommand::InitVertexAttributes(1,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Normal));
    Renderer::RendererCommand::InitVertexAttributes(2,
                                                    2,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));
    Renderer::RendererCommand::InitVertexAttributes(3,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Tangent));
    Renderer::RendererCommand::InitVertexAttributes(4,
                                                    3,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Bitangent));
    Renderer::RendererCommand::InitVertexAttributes(5,
                                                    4,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));
    Renderer::RendererCommand::InitVertexAttributes(6,
                                                    4,
                                                    Renderer::RendererAPI::NumericalDataType::Float,
                                                    Renderer::RendererAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));

    unsigned clearVAO = 0;
    Renderer::RendererCommand::BindVertexArray(clearVAO);
}

void Mesh::Draw(Renderer::Shader& shader)
{
    unsigned diffuseNr  = 1;
    unsigned specularNr = 1;
    unsigned normalNr   = 1;
    unsigned heightNr   = 1;

    for (unsigned i = 0; i < textures.size(); i++)
    {
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        Renderer::RendererCommand::BindTexture(static_cast<RendererAPI::Texture>(RendererAPI::Texture::Texture0 + i),
                                               RendererAPI::TextureTarget::Texture2D,
                                               textures[i].id);

        Renderer::RendererCommand::SetUniformInt(shader.ID, ("material." + name + number).c_str(), i);
    }

    Renderer::RendererCommand::SubmitDrawCommands(shader.ID, m_VAO, indices.size());
}

} // namespace Renderer
} // namespace Moonstone
