#include "..\headers\imgui_manager.hpp" 
#include "..\headers\scenes\introScene.hpp" 
#include "..\headers\scenes\scene1.hpp" 
#include "..\headers\scenes\scene2.hpp" 

extern HWND ghwnd; 

bool ImGuiManager::initialized = false;

// scene  
extern Scene1 scene1; 
extern Scene2 scene2; 

// intro scene related 
GLfloat cameraPosition[3] = { 4.0f, 0.0f, 6.0f };  
extern GLfloat cameraPosY; 
extern GLfloat cameraRotateAngle; 
extern GLfloat cameraRadius; 
extern IntroScene introScene;

// scene2 related 
extern float tHeight1; 
extern float tHeight2; 
extern float tHeight3; 
extern float tHeight4; 

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

    ImGui::SliderFloat("camera height", &cameraPosY, -5.0f, 5.0f);
    ImGui::SliderFloat("camera radius", &cameraRadius, 0.0f, 50.0f);
    ImGui::SliderFloat("camera angle", &cameraRotateAngle, 0.0f, 360.0f);
    
    ImGui::SliderInt("Blur iterations:", &introScene.blurIterations, 0, 25); 
    ImGui::SliderFloat("Blend Strength:", &introScene.blendStrength, 0, 10); 
} 

float lightPos1[3]; 
void scene1Controls(void) 
{
    bool isFirstTime = true; 
    if(isFirstTime == true) 
    {
        lightPos1[0] = scene1.lightPosition[0]; 
        lightPos1[1] = scene1.lightPosition[1]; 
        lightPos1[2] = scene1.lightPosition[2]; 
        isFirstTime = false; 
    }

    ImGui::SliderFloat3("light position", lightPos1, -10.0f, 10.0f);
    scene1.lightPosition[0] = lightPos1[0]; 
    scene1.lightPosition[1] = lightPos1[1]; 
    scene1.lightPosition[2] = lightPos1[2]; 
} 

void scene2Controls(void) 
{
    ImGui::SliderFloat("texture height 1", &tHeight1, -0.0f, 2000.0f);
    ImGui::SliderFloat("texture height 2", &tHeight2, -0.0f, 2000.0f);
    ImGui::SliderFloat("texture height 3", &tHeight3, -0.0f, 2000.0f);
    ImGui::SliderFloat("texture height 4", &tHeight4, -0.0f, 2000.0f);
} 

