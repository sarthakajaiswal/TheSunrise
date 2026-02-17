#include "..\headers\imgui_manager.hpp" 
#include "..\headers\scenes\introScene.hpp" 

extern HWND ghwnd; 

bool ImGuiManager::initialized = false;

GLfloat cameraPosition[3] = { 4.0f, 0.0f, 6.0f };  
extern GLfloat cameraPosY; 
extern GLfloat cameraRotateAngle; 
extern GLfloat cameraRadius; 
 
extern IntroScene introScene; 

bool ImGuiManager::Initialize(HWND hwnd) {
    if (initialized || !ENABLE_IMGUI)
        return false;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplOpenGL3_Init("#version 130");

    initialized = true;
    return true;
}

void ImGuiManager::Cleanup() {
    if (!initialized || !ENABLE_IMGUI)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    initialized = false;
}

void ImGuiManager::NewFrame() {
    if (!initialized || !ENABLE_IMGUI)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::Render() {
    if (!initialized || !ENABLE_IMGUI)
        return;

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::RenderDebugWindow() {
    if (!initialized || !ENABLE_IMGUI)
        return;

    // Create a debug window
    ImGui::Begin("Debug Window");
    
    // Add any debug information you want to display
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::SliderFloat("camera height", &cameraPosY, -5.0f, 5.0f);
    ImGui::SliderFloat("camera radius", &cameraRadius, 0.0f, 20.0f);
    ImGui::SliderFloat("camera angle", &cameraRotateAngle, 0.0f, 360.0f);
    
    ImGui::Checkbox("Normal mapping:", &introScene.bNormalMapping); 
    ImGui::SliderInt("Blur iterations:", &introScene.blurIterations, 0, 25); 
    ImGui::SliderFloat("Blend Strength:", &introScene.blendStrength, 0, 10); 

    ImGui::End();
}

