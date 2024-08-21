#include "Include/ImGuiLayer.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Include/Application.h"

namespace Moonstone
{

namespace Tools
{

/**
 * @brief Constructs an ImGuiLayer instance.
 *
 * Initializes the layer with the name "ImGuiLayer" and sets the window pointer to nullptr.
 */
ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
    , m_Window(nullptr)
{
}

/**
 * @brief Sets the GLFW window for the ImGuiLayer.
 *
 * This function associates a GLFW window with the ImGuiLayer, allowing it to
 * render on the specified window.
 *
 * @param window A pointer to the GLFW window to set.
 */
void ImGuiLayer::SetWindow(GLFWwindow* window) { m_Window = window; }

/**
 * @brief Attaches the ImGuiLayer.
 *
 * Initializes ImGui context and configuration.
 * Sets up ImGui for use with OpenGL and GLFW.
 * Logs an error if the GLFW window is not set.
 */
void ImGuiLayer::OnAttach()
{
    if (!m_Window)
    {
        MS_ERROR("imgui cannot get the glfw window");
        return;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsClassic();
    //ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // ToDo: Abstract this out for Vulks

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

/**
 * @brief Detaches the ImGuiLayer.
 *
 * Cleans up ImGui resources and shuts down ImGui for OpenGL and GLFW.
 */
void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * @brief Starts a new ImGui frame.
 *
 * Prepares ImGui for a new frame by initializing the frame-specific resources
 * for OpenGL and GLFW.
 */
void ImGuiLayer::Start()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/**
 * @brief Ends the current ImGui frame.
 *
 * Renders the current ImGui frame and draws the data using OpenGL.
 */
void ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * @brief Handles ImGui rendering.
 *
 * This function is a placeholder for custom ImGui rendering code.
 */
void ImGuiLayer::OnImGuiRender() {}

} // namespace Tools

} // namespace Moonstone
