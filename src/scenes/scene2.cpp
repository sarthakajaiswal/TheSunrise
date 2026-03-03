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
    // // code 
    // logFile.log("------------------ Scene2::initialize() started ----------------\n"); 

    // std::vector<std::string> terrainTextureImages = 
    // {
    //     "res/terrain1.png", 
    //     "res/terrain2.png", 
    //     "res/terrain3.png", 
    //     "res/terrain4.png" 
    // }; 
	// terrain.initialize(terrainScale, heightScale, textureScale, terrainTextureImages); 

    // glFrontFace(GL_CW); 

    // logFile.log("Scene2::initialize() > terrain initialized\n"); 

    // scene2Camera.setState(vec3(cameraX, cameraY, cameraZ)); 


    // logFile.log("------------------ Scene2::initialize() completed ----------------\n\n"); 
    return (0); 
} 

float tHeight1 = 20.0;  // texture height 1 
float tHeight2 = 50.0;  
float tHeight3 = 100.0;  
float tHeight4 = 150.0;  
void Scene2::display() 
{
   mat4 modelMatrix = mat4::identity(); 
   viewMatrix = mat4::identity(); 
//    viewMatrix = scene2Camera.getViewMatrix(); 

//    terrain.SetTextureHeights(tHeight1, tHeight2, tHeight3, tHeight4); 
//    terrain.Render(modelMatrix, viewMatrix, projectionMatrix); 
} 

void Scene2::update() 
{
    // code 
} 

Scene2::~Scene2() 
{
} 

void scene2Callbacks(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
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

