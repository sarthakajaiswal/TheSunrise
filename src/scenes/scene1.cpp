#include "../../headers/scenes/scene1.hpp" 

/* 
388.0f, 46.00f, 501.85f 
439.514f, 18.81f, 1337.290f
650.896f, 18.81f, 1337.290f | 0.79, 0.59, 0.044 
*/ 

Camera scene1Camera; 
// float cubemapYAngle=178.0f; 
float modelX = 534.06, modelY = 46.0, modelZ = 501.85; 
float modelSx = 1.0, modelSy = 1.0, modelSz = 1.0; 

Scene1::Scene1() 
{
    // code  
    exposureValue = 0.2f; 
} 

int Scene1::initialize() 
{	
    // code 
    logFile.log("------------------ Scene1::initialize() started ----------------\n"); 

    // -------- Effects --------- 
    // assert(exposureProgram.initialize() == 0); 
    // assert(fsTexturer.initialize() == 0); 

    std::vector<std::string> textureImages = {"res/terrain1.png", "res/terrain3.png", "res/terrain2.png", "res/terrain4.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/scene1terrain.png", 1.0f, 60.0f, textureImages, textureHeightRanges, 4.0); 

    // const char* cubemapImages[6] = 
    // {
	// 	"res/environments/environment1/px.png", 
	// 	"res/environments/environment1/nx.png", 
	// 	"res/environments/environment1/py.png", 
	// 	"res/environments/environment1/ny.png", 
	// 	"res/environments/environment1/pz.png", 
	// 	"res/environments/environment1/nz.png" 
    // }; 
    // cubemap.initialize(cubemapImages); 

    // -------- Objects --------- 
    // assert(quad.initialize() == 0); 
    logFile.log("Loading Model...\n"); 
    assert(treeModel.initialize("res/models/game_tree/scene.gltf") == 0); 
    logFile.log("Model Loaded\n"); 

    // -------- FBO and textures --------- 
    // assert(floatingPointFBO.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    // quoteTexture = loadTexture("res\\phrase1.png", FALSE);
    // if(quoteTexture == 0) 
    //     throw texture_loading_failure("Scene1::initialize() > phrase1 texture loading failed\n");  

    // gateTexture = loadTexture("res\\gate.png"); 
    // if(gateTexture == 0) 
    //     throw texture_loading_failure("Scene1::initialize() > gate texture loading failed\n");
        
    // quoteTexture = loadTexture("res\\phrase1.png"); 
    // if(quoteTexture == 0) 
    //     throw texture_loading_failure("Scene1::initialize() > phrase1 texture loading failed\n");

    scene1Camera.setState(vec3(534.06, 46.83, 501.85), 88.60, 1.20); 

    logFile.log("------------------ Scene1::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void Scene1::uninitialize()  
{
    // code 
    logFile.log("Scene1::uninitialize() > Uninitializing...\n"); 
    if(gateTexture != 0) 
    {
        glDeleteTextures(1, &gateTexture); 
        gateTexture = 0; 
    } 

    if(quoteTexture != 0) 
    {
        glDeleteTextures(1, &quoteTexture); 
        quoteTexture = 0; 
    } 
    
    logFile.log("Scene1::uninitialize() > Uninitialized\n"); 
} 

void Scene1::display() 
{
    // floatingPointFBO.bind(); 
    // fsTexturer.render(gateTexture); 
    // floatingPointFBO.unbind(); 

    // exposureProgram.render(floatingPointFBO.getTextureID(), exposureValue); 

    // glEnable(GL_BLEND); 
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    // fsTexturer.render(quoteTexture); 
    // glDisable(GL_BLEND); 

    // ---------- 

    viewMatrix = scene1Camera.getViewMatrix(CAMERA_GAME_MODE); 
    terrain.render(mat4::identity(), viewMatrix, projectionMatrix, scene1Camera.getPosition()); 

    // mat4 rotationMatrix = vmath::rotate(178.0f, 0.0f, 1.0f, 0.0f); 
    // rotationMatrix = rotationMatrix * vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f); 
    // cubemap.render(rotationMatrix, viewMatrix, projectionMatrix); 

    mat4 modelMatrix = mat4::identity(); 
    // modelMatrix *= vmath::translate(modelX, modelY, modelZ); 
    // modelMatrix *= vmath::scale(modelSx, modelSy, modelSz);
    modelMatrix *= vmath::translate(650.896f, 18.81f, 1337.290f); 
    modelMatrix *= vmath::scale(0.79f, 0.59f, 0.044f);
    treeModel.draw(modelMatrix, viewMatrix, projectionMatrix); 
} 

void Scene1::update() 
{
    // code 
} 

Scene1::~Scene1() 
{
} 

void Scene1::scene1Callbacks(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    bool doImGuiCapturedEvent = false; 
    if(ImGuiManager::initialized == true)
    {
        ImGuiIO& io = ImGui::GetIO(); 
        doImGuiCapturedEvent = io.WantCaptureMouse; 
    } 

    if(!doImGuiCapturedEvent) 
        scene1Camera.cameraCallback(hwnd, uMsg, wParam, lParam);  

    switch(uMsg) 
    {
        case WM_CHAR: 
            switch(wParam) 
            {
            default: 
                break; 
            } 
            break; 

        default: 
            break; 
    } 
} 

