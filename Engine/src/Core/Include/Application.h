#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Include/Core.h"
#include "Core/Include/Logger.h"
#include "Core/Include/Window.h"
#include "Renderer/Include/Camera.h"
#include "Renderer/Include/Scene.h"
#include "Renderer/Include/Shader.h"
#include "Renderer/Include/Textures.h"
#include "Tools/Include/BaseShapes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Moonstone
{

namespace Core
{

class Application
{
    public:
        Application();
        virtual ~Application() = default;

        void Run();

        inline static std::unique_ptr<Application> &GetApplicationInstance() { return s_ApplicationInstance; }

        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);
        void PushOverlay(Layer *overlay);
        void PopOverlay(Layer *overlay);

        inline bool GetDefaultGridEnabled() { return m_DefaultGrid; }
        inline void SetDefaultGridEnabled(bool show) { m_DefaultGrid = show; }
        inline void ToggleSunlight() { m_SunLight = !m_SunLight; }

    private:
        void RenderLayers();
        void InitializeImGui();
        void InitializeDefaultScene(std::vector<unsigned> &shaderProgram,
                                    std::vector<unsigned> &VBO,
                                    std::vector<unsigned> &VAO,
                                    std::vector<unsigned> &EBO,
                                    std::vector<unsigned> &textures);

        void AddCube(std::vector<unsigned> &shaderProgram,
                     std::vector<unsigned> &VBO,
                     std::vector<unsigned> &VAO,
                     std::vector<unsigned> &EBO,
                     std::vector<unsigned> &textures);

        void AddPyramid(std::vector<unsigned> &shaderProgram,
                        std::vector<unsigned> &VBO,
                        std::vector<unsigned> &VAO,
                        std::vector<unsigned> &EBO,
                        std::vector<unsigned> &textures);

    private:
        bool                     m_Running;
        static std::unique_ptr<Application>       s_ApplicationInstance;
        LayerStack                                m_LayerStack;
        std::shared_ptr<Window>  m_Window;
        Tools::ImGuiLayer                        *m_ImGuiLayer;
        std::vector<Layer>                        m_Layers;
        bool                     m_DefaultGrid = true;
        std::vector<Renderer::Scene::SceneObject> m_Objects;
        bool                     m_SunLight  = false;
        glm::vec3                m_TimeOfDay = {-1.0f, -0.5f, 0};

        std::vector<unsigned> m_ShaderProgram, m_VBO, m_VAO, m_EBO, m_Texture;
};

std::unique_ptr<Application> CreateApplicationInstance();

} // namespace Core

} // namespace Moonstone

#endif // APPLICATION_H
