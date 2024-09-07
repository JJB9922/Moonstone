#include "Include/Model.h"
#include "assimp/postprocess.h"

namespace Moonstone
{

namespace Rendering
{

void Model::Draw(Rendering::Shader &shader)
{
    for (auto &mesh : m_Meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(std::string &path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                     aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MS_ERROR("assimp error: {0}", import.GetErrorString());
        return;
    }

    MS_DEBUG("imported model succesfully");
    m_Directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode *node, const aiScene *scene)
{
    if (!node || !scene)
        return;

    for (unsigned i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        if (mesh)
            m_Meshes.push_back(ProcessMesh(mesh, scene));
    }

    for (unsigned i = 0; i < node->mNumChildren; i++)
    {
        if (node->mChildren[i])
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }
}

Mesh Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<unsigned> indices;
    std::vector<Mesh::Texture> textures;

    for (unsigned i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;

        glm::vec3 vector;
        // Pos
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Norm
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Tex
        if (mesh->mTextureCoords[0])
        {
            glm::vec3 vector;
            vector.x = mesh->mTextureCoords[0][i].x;
            vector.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vector;

            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }
        else
        {
            vertex.TexCoords = glm::vec2(0.0, 0.0);
        }

        vertices.push_back(vertex);
    }

    for (unsigned i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Mesh::Texture> diffuseMaps =
            LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Mesh::Texture> specularMaps =
            LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<Mesh::Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        std::vector<Mesh::Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Mesh::Texture> textures;
    for (unsigned i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned j = 0; j < m_TexturesLoaded.size(); j++)
        {
            if (std::strcmp(m_TexturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(m_TexturesLoaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Mesh::Texture texture;
            texture.id = TextureFromFile(str.C_Str(), m_Directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            m_TexturesLoaded.push_back(texture);
        }
    }

    return textures;
}

unsigned Model::TextureFromFile(const std::string &path, const std::string &directory, bool gamma)
{
    std::string filename = path;
    filename = directory + '/' + filename;

    unsigned textureID;
    Rendering::RenderingCommand::CreateTexture(textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        RenderingAPI::TextureFormat format;
        if (nrComponents == 1)
        {
            format = RenderingAPI::TextureFormat::Red;
        }
        else if (nrComponents == 3)
        {
            format = RenderingAPI::TextureFormat::RGB;
        }
        else if (nrComponents == 4)
        {
            format = RenderingAPI::TextureFormat::RGBA;
        }

        Rendering::RenderingCommand::UploadTexture(Rendering::RenderingAPI::TextureTarget::Texture2D, 0, format, width,
                                                   height, format, RenderingAPI::NumericalDataType::UnsignedByte, data);

        RenderingCommand::SetTextureParameters(RenderingAPI::TextureTarget::Texture2D,
                                               RenderingAPI::TextureParameterName::TextureWrapS,
                                               RenderingAPI::TextureParameter::Repeat);
        RenderingCommand::SetTextureParameters(RenderingAPI::TextureTarget::Texture2D,
                                               RenderingAPI::TextureParameterName::TextureWrapT,
                                               RenderingAPI::TextureParameter::Repeat);
        RenderingCommand::SetTextureParameters(RenderingAPI::TextureTarget::Texture2D,
                                               RenderingAPI::TextureParameterName::TextureFilteringMin,
                                               RenderingAPI::TextureParameter::LinearMipmapLinear);
        RenderingCommand::SetTextureParameters(RenderingAPI::TextureTarget::Texture2D,
                                               RenderingAPI::TextureParameterName::TextureFilteringMag,
                                               RenderingAPI::TextureParameter::Linear);

        stbi_image_free(data);
        MS_DEBUG("Loaded texture: width={0}, height={1}, components={2}", width, height, nrComponents);
    }
    else
    {
        MS_ERROR("texture failed to load: {0}", path);
        stbi_image_free(data);
    }

    return textureID;
}

} // namespace Rendering

} // namespace Moonstone
