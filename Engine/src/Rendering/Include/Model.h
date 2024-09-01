#ifndef MODEL_H
#define MODEL_H

#include "Rendering/Include/Mesh.h"
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
        Model(std::string &path) { LoadModel(path); };
        void Draw(Rendering::Shader &shader);

    private:
        void                       LoadModel(std::string &path);
        void                       ProcessNode(aiNode *node, const aiScene *scene);
        Mesh                       ProcessMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Mesh::Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
        unsigned TextureFromFile(const std::string &path, const std::string &directory, bool gamma = false);

    private:
        std::vector<Mesh>          m_Meshes;
        std::string                m_Directory;
        std::vector<Mesh::Texture> m_TexturesLoaded;
};

} // namespace Rendering

} // namespace Moonstone

#endif // MODEL_H
