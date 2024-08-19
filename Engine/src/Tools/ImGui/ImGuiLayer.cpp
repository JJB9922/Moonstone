#include "Include/ImGuiLayer.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Core/Include/Application.h"

namespace Moonstone
{

namespace Tools
{

ImGuiLayer::ImGuiLayer()
    : Layer("ImGuiLayer")
    , m_Window(nullptr)
{
}

void ImGuiLayer::SetWindow(GLFWwindow* window) { m_Window = window; }

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

    // ToDo: Abstract this out for Vulks

    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void ImGuiLayer::OnDetach()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayer::Start()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnImGuiRender() {}

} // namespace Tools

} // namespace Moonstone
