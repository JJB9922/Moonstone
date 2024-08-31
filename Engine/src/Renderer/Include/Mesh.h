#ifndef MESH_H
#define MESH_H

#include "Renderer/Include/Shader.h"
#include "mspch.h"
#include <glm/glm.hpp>

namespace Moonstone
{

namespace Renderer
{

class Mesh
{
    public:
        struct Vertex
        {
                glm::vec3 Position;
                glm::vec3 Normal;
                glm::vec2 TexCoords;
        };

        struct Texture
        {
                unsigned    id;
                std::string type;
                std::string path;
        };

    public:
        std::vector<Vertex>   vertices;
        std::vector<unsigned> indices;
        std::vector<Texture>  textures;

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
            : vertices(std::move(vertices))
            , indices(std::move(indices))
            , textures(std::move(textures))
        {
            SetupMesh();
        }

        void Draw(Shader &shader);

    private:
        void SetupMesh();

    private:
        unsigned m_VAO, m_VBO, m_EBO;
};

} // namespace Renderer

} // namespace Moonstone

#endif // MESH_H
