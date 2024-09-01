#include "Include/Mesh.h"

namespace Moonstone
{

namespace Rendering
{

void Mesh::SetupMesh()
{
    Rendering::RenderingCommand::InitVertexArray(m_VAO);
    Rendering::RenderingCommand::InitVertexBuffer(m_VBO,
                                                reinterpret_cast<float*>(vertices.data()),
                                                vertices.size() * sizeof(Vertex));
    Rendering::RenderingCommand::InitElementBuffer(m_EBO, indices.data(), indices.size() * sizeof(unsigned));

    Rendering::RenderingCommand::InitVertexAttributes(0,
                                                    3,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    0);
    Rendering::RenderingCommand::InitVertexAttributes(1,
                                                    3,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Normal));
    Rendering::RenderingCommand::InitVertexAttributes(2,
                                                    2,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));
    Rendering::RenderingCommand::InitVertexAttributes(3,
                                                    3,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Tangent));
    Rendering::RenderingCommand::InitVertexAttributes(4,
                                                    3,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, Bitangent));
    Rendering::RenderingCommand::InitVertexAttributes(5,
                                                    4,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));
    Rendering::RenderingCommand::InitVertexAttributes(6,
                                                    4,
                                                    Rendering::RenderingAPI::NumericalDataType::Float,
                                                    Rendering::RenderingAPI::BooleanDataType::False,
                                                    sizeof(Vertex),
                                                    offsetof(Vertex, TexCoords));

    unsigned clearVAO = 0;
    Rendering::RenderingCommand::BindVertexArray(clearVAO);
}

void Mesh::Draw(Rendering::Shader& shader)
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

        Rendering::RenderingCommand::BindTexture(static_cast<RenderingAPI::Texture>(RenderingAPI::Texture::Texture0 + i),
                                               RenderingAPI::TextureTarget::Texture2D,
                                               textures[i].id);

        Rendering::RenderingCommand::SetUniformInt(shader.ID, ("material." + name + number).c_str(), i);
    }

    Rendering::RenderingCommand::SubmitDrawCommands(shader.ID, m_VAO, indices.size());
}

} // namespace Rendering
} // namespace Moonstone
