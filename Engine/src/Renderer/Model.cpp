#include "Include/Model.h"

namespace Moonstone
{

namespace Renderer
{

void Model::Draw(Renderer::Shader& shader)
{
    for (auto& mesh : m_Meshes)
    {
        mesh.Draw(shader);
    }
}

void Model::LoadModel(std::string& path)
{
    Assimp::Importer import;
    const aiScene*   scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        MS_ERROR("assimp error: {0}", import.GetErrorString());
        return;
    }

    MS_DEBUG("imported model succesfully");
    m_Directory = path.substr(0, path.find_last_of('/'));
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    if (!node || !scene)
        return;

    for (unsigned i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
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

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Mesh::Vertex>  vertices;
    std::vector<unsigned>      indices;
    std::vector<Mesh::Texture> textures;

    for (unsigned i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;

        glm::vec3 vector;
        // Pos
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Norm
        vector.x      = mesh->mNormals[i].x;
        vector.y      = mesh->mNormals[i].y;
        vector.z      = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // Tex
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vector;
            vector.x         = mesh->mTextureCoords[0][i].x;
            vector.y         = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vector;
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
        aiMaterial*                material    = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<Mesh::Texture> diffuseMaps = LoadMaterialTextures(material,
                                                                      aiTextureType_DIFFUSE,
                                                                      "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<Mesh::Texture> specularMaps = LoadMaterialTextures(material,
                                                                       aiTextureType_SPECULAR,
                                                                       "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Mesh::Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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
            texture.id   = TextureFromFile(str.C_Str(), m_Directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
    }

    return textures;
}

unsigned Model::TextureFromFile(const std::string& path, const std::string& directory, bool gamma)
{
    std::string filename = path;
    filename             = directory + '/' + filename;

    unsigned textureID;
    Renderer::RendererCommand::CreateTexture(textureID);

    int            width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        RendererAPI::TextureFormat format;
        if (nrComponents == 1)
        {
            format = RendererAPI::TextureFormat::Red;
        }
        else if (nrComponents == 3)
        {
            format = RendererAPI::TextureFormat::RGB;
        }
        else if (nrComponents == 4)
        {
            format = RendererAPI::TextureFormat::RGBA;
        }

        Renderer::RendererCommand::UploadTexture(Renderer::RendererAPI::TextureTarget::Texture2D,
                                                 0,
                                                 format,
                                                 width,
                                                 height,
                                                 format,
                                                 RendererAPI::NumericalDataType::UnsignedByte,
                                                 data);

        RendererCommand::SetTextureParameters(RendererAPI::TextureTarget::Texture2D,
                                              RendererAPI::TextureParameterName::TextureWrapS,
                                              RendererAPI::TextureParameter::Repeat);
        RendererCommand::SetTextureParameters(RendererAPI::TextureTarget::Texture2D,
                                              RendererAPI::TextureParameterName::TextureWrapT,
                                              RendererAPI::TextureParameter::Repeat);
        RendererCommand::SetTextureParameters(RendererAPI::TextureTarget::Texture2D,
                                              RendererAPI::TextureParameterName::TextureFilteringMin,
                                              RendererAPI::TextureParameter::LinearMipmapLinear);
        RendererCommand::SetTextureParameters(RendererAPI::TextureTarget::Texture2D,
                                              RendererAPI::TextureParameterName::TextureFilteringMag,
                                              RendererAPI::TextureParameter::Linear);

        stbi_image_free(data);
    }
    else
    {
        MS_ERROR("texture failed to load: {0}", path);
        stbi_image_free(data);
    }

    return textureID;
}

} // namespace Renderer

} // namespace Moonstone
