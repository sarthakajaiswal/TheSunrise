#include "../../headers/scenes/outroScene.hpp" 

extern BOOL bDone; 

static bool bShowPoster = false; 
static float posterAlpha = 1.0f; 

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
        0.9, 3.0); 

    texture_theSystemCallsPoster = loadTexture("res/textures/outro/theSystemCallsPoster.png"); 

    logFile.log("------------------ OutroScene::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void OutroScene::display() 
{
    static int slideRenderingCompleted = 0; 

    if(slideRenderingCompleted == 0 && bShowPoster == false) 
        slideRenderingCompleted = slidesSequence.render(); 

    if(bShowPoster == true) 
        fsTexturer.render(texture_theSystemCallsPoster, posterAlpha); 
} 

void OutroScene::update() 
{
    slidesSequence.update(); 

    if(mainTimer > 165.0) 
        bShowPoster = true; 
    if(mainTimer > 168) 
        posterAlpha -= 0.01f; 

    if(mainTimer > 186.0) 
        bDone = TRUE; 
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
