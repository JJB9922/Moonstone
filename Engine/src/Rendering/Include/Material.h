#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/glm.hpp>

namespace Moonstone
{

namespace Rendering
{

class Material
{
  public:
    struct Mat
    {
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;

        glm::vec3 baseColour;

        Mat() : diffuse(0.0f, 0.0f, 0.0f), specular(0.0f, 0.0f, 0.0f), shininess(1.0f), baseColour(0.0f, 1.0f, 0.0f)
        {
        }

        Mat(const glm::vec3 &diffuse, const glm::vec3 &specular, const float &shininess, const glm::vec3 &baseColour)
            : diffuse(diffuse), specular(specular), shininess(shininess), baseColour(baseColour)
        {
        }
    };

    Material() = default;
    ~Material() = default;
};

} // namespace Rendering

} // namespace Moonstone

#endif // MATERIAL_H
