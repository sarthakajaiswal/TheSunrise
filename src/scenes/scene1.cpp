#include "../../headers/scenes/scene1.hpp" 

Camera scene1Camera; 

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
    assert(fsTexturer.initialize() == 0); 

    std::vector<std::string> textureImages = {"res/terrain1.png", "res/terrain3.png", "res/terrain2.png", "res/terrain4.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/scene1terrain.png", 1.0f, 70.0f, textureImages, textureHeightRanges, 8.0); 

    // -------- Objects --------- 
    assert(quad.initialize() == 0); 

    // -------- FBO and textures --------- 
    assert(floatingPointFBO.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    quoteTexture = loadTexture("res\\phrase1.png", FALSE);
    if(quoteTexture == 0) 
        throw texture_loading_failure("Scene1::initialize() > phrase1 texture loading failed\n");  

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

