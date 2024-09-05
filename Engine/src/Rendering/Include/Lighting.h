#ifndef LIGHTING_H
#define LIGHTING_H

#include "mspch.h"
#include <glm/glm.hpp>
#include <string>

namespace Moonstone
{

namespace Rendering
{

class Lighting
{
  public:
    enum class LightType
    {
        Directional,
        Point,
    };
    struct Light
    {
        // TODO not a string anymore pls
        std::string id;

        LightType type;

        glm::vec3 direction, ambient, diffuse, specular;
        float constant, linear, quadratic;

        glm::vec3 position;
        bool isActive;

        Light(const std::string &id, const glm::vec3 &dir, const glm::vec3 &amb, const glm::vec3 &diff,
              const glm::vec3 &spec, bool active)
            : id(id), type(LightType::Directional), direction(dir), ambient(amb), diffuse(diff), specular(spec),
              isActive(active)
        {
        }

        Light(const std::string &id, const glm::vec3 &pos, const glm::vec3 &amb, const glm::vec3 &diff,
              const glm::vec3 &spec, const bool &active, const float &constant, const float &linear,
              const float &quadratic)
            : id(id), type(LightType::Point), position(pos), ambient(amb), diffuse(diff), specular(spec),
              isActive(active), constant(constant), linear(linear), quadratic(quadratic)
        {
        }
    };

    Lighting() = default;
    ~Lighting() = default;

    inline void AddLight(Light &light)
    {
        m_Lights.push_back(light);
    }

    inline std::vector<Light> GetLights()
    {
        return m_Lights;
    }

    inline void SetLight(std::string &id, Light updatedLight)
    {
        for (auto &light : m_Lights)
        {
            if (light.id == id)
            {
                light = updatedLight;
            }
        }
    }

  private:
    std::vector<Light> m_Lights;
};

} // namespace Rendering

} // namespace Moonstone

#endif // LIGHTING_H
