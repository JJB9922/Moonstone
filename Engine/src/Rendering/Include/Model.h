#ifndef MODEL_H
#define MODEL_H

#include "Rendering/Include/Mesh.h"
#include "Rendering/Include/Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

namespace Moonstone
{

namespace Rendering
{

class Model
{
  public:
    Model(const std::string &id, const Shader &shader, const glm::vec3 &position, const glm::vec3 &rotation,
          const glm::vec3 &scale, std::string &path)
        : id(id), shader(shader), position(position), rotation(rotation), scale(scale)
    {
        LoadModel(path);
    };

    Model() = default;

    void Draw(Rendering::Shader &shader);
    inline void Clear()
    {
        id.clear();
        position = rotation = glm::vec3(0);
        scale = glm::vec3(1);
    }

  private:
    void LoadModel(std::string &path);
    void ProcessNode(aiNode *node, const aiScene *scene);
    Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Mesh::Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned TextureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

  public:
    std::string id;
    Shader shader;
    glm::vec3 position, rotation, scale;

  private:
    std::vector<Mesh> m_Meshes;
    std::string m_Directory;
    std::vector<Mesh::Texture> m_TexturesLoaded;
};

} // namespace Rendering

} // namespace Moonstone

#endif // MODEL_H
