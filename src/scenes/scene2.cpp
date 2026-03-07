#include "../../headers/scenes/scene2.hpp" 

Scene2::Scene2() 
{
    // code  
} 

Camera scene2Camera; 
float cameraX = 5.0; 
float cameraY = 300.0; 
float cameraZ = 10.0; 

int Scene2::initialize() 
{	
    // code 
    logFile.log("------------------ Scene2::initialize() started ----------------\n"); 

    std::vector<std::string> textureImages = {"res/terrain5.png", "res/terrain6.png", "res/terrain7.png", "res/terrain8.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/scene2terrain.png", 1.5f, 60.0f, textureImages, textureHeightRanges, 8.0);

    scene2Camera.setState(vec3(270.33, 50.34, 325.60), 249.50, -9.70); 


    logFile.log("------------------ Scene2::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void Scene2::display() 
{
   mat4 modelMatrix = mat4::identity(); 
   viewMatrix = mat4::identity(); 
   viewMatrix = scene2Camera.getViewMatrix(CAMERA_GAME_MODE); 
 
   terrain.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition()); 
} 

void Scene2::update() 
{
    // code 
} 

Scene2::~Scene2() 
{
} 

void Scene2::eveneCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    // variable declarations 
    float cameraStep = 0.5f; 
    
    // code 
    scene2Camera.cameraCallback(hwnd, uMsg, wParam, lParam); 
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

