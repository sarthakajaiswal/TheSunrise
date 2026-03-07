#include "..\headers\imgui_manager.hpp" 
#include "..\headers\scenes\introScene.hpp" 
#include "..\headers\scenes\scene1.hpp" 
#include "..\headers\scenes\scene2.hpp" 

extern HWND ghwnd; 

bool ImGuiManager::initialized = false;

// scene  
extern IntroScene introScene; 
extern Scene1 scene1; 
extern Scene2 scene2; 

// scene1 related 
// extern float cubemapYAngle; 
extern float modelX, modelY, modelZ; 
extern float modelSx, modelSy, modelSz;
extern float tx, ty, tz; 
extern float sx, sy, sz; 
extern float rx, ry, rz; 
extern float alpha; 

// scene2 related 

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
    // function declarations 
    void introSceneControls(void);     
    void scene1Controls(void);     
    void scene2Controls(void);     

    // code 
    if (!initialized || !ENABLE_IMGUI)
        return;

    // Create a debug window
    ImGui::Begin("Debug Window");
    
    // Add any debug information you want to display
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    switch(CurrentScene) 
    {
        case INTRO_SCENE: 
            introSceneControls(); 

        case SCENE_1: 
            scene1Controls(); 
            break; 

        case SCENE_2: 
            scene2Controls(); 
            break; 

        default: 
            break; 
    } 

    ImGui::End();
}

// float lightPos[] = {introScene.lightPosition[0], introScene.lightPosition[1], introScene.lightPosition[2]}; 
void introSceneControls(void) 
{
    ImGui::SliderFloat("alphabet spacing", &introScene.alphabetSpacing, 0.0f, 5.0f);
    ImGui::SliderFloat("amc startX", &introScene.astrmomedicompStartingX, -40.0f, 0.0f);
    ImGui::SliderFloat("alphabet Sx", &introScene.alphabetSx, -3.0f, 3.0f);
    ImGui::SliderFloat("alphabet Sy", &introScene.alphabetSy, -3.0f, 3.0f);
    ImGui::SliderFloat("alphabet Sz", &introScene.alphabetSz, -3.0f, 3.0f);
    ImGui::SliderFloat("amcZ", &introScene.astromedicompZ, 0.0f, -50.0f);

    // ImGui::SliderFloat3("light position", lightPos, -50.0f, 50.0f);
    // introScene.lightPosition[0] = lightPos[0]; 
    // introScene.lightPosition[1] = lightPos[1]; 
    // introScene.lightPosition[2] = lightPos[2]; 

    ImGui::SliderInt("Blur iterations:", &introScene.blurIterations, 0, 25); 
    ImGui::SliderFloat("Blend Strength:", &introScene.blendStrength, 0, 10); 
} 

void scene1Controls(void) 
{
    // ImGui::SliderFloat("exposure:", &scene1.exposureValue, 0.0, 5.0);  
    // ImGui::SliderFloat("dx", &modelX, -1000.0f, 1000.0f);
    // ImGui::SliderFloat("dy", &modelY, -1000.0f, 1000.0f);
    // ImGui::SliderFloat("dz", &modelZ, 0.0f, 2000.0f);
    // ImGui::SliderFloat("dsx", &modelSx, 0.0f, 1.0f);
    // ImGui::SliderFloat("dsy", &modelSy, 0.0f, 1.0f);
    // ImGui::SliderFloat("dsz", &modelSz, 0.0f, 1.0f);
    // ImGui::SliderFloat("cubemap Y angle:", &cubemapYAngle, 0.0, 360.0);  

    ImGui::SliderFloat("tx", &tx, -500.0f, 1000.0f);
    ImGui::SliderFloat("ty", &ty, -500.0f, 1000.0f);
    ImGui::SliderFloat("tz", &tz, -500.0f, 2000.0f);
    
    ImGui::SliderFloat("sx", &sx, 0.0f, 100.0f);
    ImGui::SliderFloat("sy", &sy, 0.0f, 100.0f);
    ImGui::SliderFloat("sz", &sz, 0.0f, 100.0f);

    ImGui::SliderFloat("rx", &rx, 0.0f, 360.0f);
    ImGui::SliderFloat("ry", &ry, 0.0f, 360.0f);
    ImGui::SliderFloat("rz", &rz, 0.0f, 360.0f);


    ImGui::SliderFloat("exposure", &scene1.exposure_godrays, 0.0f, 1.0f);
    ImGui::SliderFloat("decay", &scene1.decay_godrays, 0.0f, 1.0f);
    ImGui::SliderFloat("density", &scene1.density_godrays, 0.0f, 1.0f);
    ImGui::SliderFloat("weight", &scene1.weight_godrays, 0.0f, 1.0f);
    ImGui::SliderInt("num samples", &scene1.numSamples_godrays, 0.0f, 100);
    ImGui::Spacing(); 
    ImGui::SliderFloat("alpha", &alpha, 0.0f, 1.0f);
} 

void scene2Controls(void) 
{
} 

