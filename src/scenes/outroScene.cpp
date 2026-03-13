#include "../../headers/scenes/outroScene.hpp" 

OutroScene::OutroScene() 
{
    // code 
} 

int OutroScene::initialize() 
{
    // code 
    logFile.log("------------------ OutroScene::initialize() started ----------------\n"); 

    fsTexturer.initialize(); 

    slidesSequence.initialize(
        {"res/textures/outro/presentedBy.png", "res/textures/outro/forYogeshwarSir.png", "res/textures/outro/onOccasionOf.png", "res/textures/outro/thankYou.png"}, 
        0.5, 8.0); 

    texture_theSystemCallsPoster = loadTexture("res/textures/outro/theSystemCallsPoster.png"); 

    logFile.log("------------------ OutroScene::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void OutroScene::display() 
{
    static int slideRenderingCompleted = 0; 

    if(slideRenderingCompleted == 0) 
        slideRenderingCompleted = slidesSequence.render(); 

    // if(mainTimer > 120.0) 
        // fsTexturer.render(texture_theSystemCallsPoster); 
} 

void OutroScene::update() 
{
    slidesSequence.update(); 
} 

void OutroScene::uninitialize() 
{
} 

void OutroScene::eveneCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
} 

OutroScene::~OutroScene() 
{
    // code 
} 
