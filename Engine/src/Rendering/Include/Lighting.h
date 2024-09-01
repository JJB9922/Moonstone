#ifndef LIGHTING_H
#define LIGHTING_H

#include "mspch.h"
#include <glm/glm.hpp>

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
                LightType type;

                float direction;

                glm::vec3 ambient, diffuse, specular;

                glm::vec3 position;
                bool      isActive;

                Light(float dir, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, bool active)
                    : type(LightType::Directional)
                    , direction(dir)
                    , ambient(amb)
                    , diffuse(diff)
                    , specular(spec)
                    , isActive(active)
                {
                }

                Light(const glm::vec3& pos,
                      const glm::vec3& amb,
                      const glm::vec3& diff,
                      const glm::vec3& spec,
                      bool             active)
                    : type(LightType::Point)
                    , position(pos)
                    , ambient(amb)
                    , diffuse(diff)
                    , specular(spec)
                    , isActive(active)
                {
                }
        };

        Lighting()  = default;
        ~Lighting() = default;

        inline void               AddLight(Light& light) { m_Lights.push_back(light); }
        inline std::vector<Light> GetLights() { return m_Lights; }

    private:
        std::vector<Light> m_Lights;
};

} // namespace Rendering

} // namespace Moonstone

#endif // LIGHTING_H
