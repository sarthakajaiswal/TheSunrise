#include "..\headers\slides.hpp" 

float slideAlpha = 0.0; 

Slides::Slides()
{
} 

int 
Slides::initialize(std::vector<const char*> _textureImages, float _fadeSpeed, float _slideDisplayTime) 
{
    logFile.log("---- Slides::initialize() started ----\n");
    assert(_textureImages.size() != 0); 

    fsTexturer.initialize(); 
    fadeSpeed = _fadeSpeed; 
    textureDisplayTime = _slideDisplayTime; 

    for(int i = 0; i < _textureImages.size(); ++i) 
        textures.push_back(loadTexture(_textureImages[i]));

    logFile.log("---- Slides::initialize() completed ----\n"); 
    return (0); 
} 

int 
Slides::render() 
{
    if(currentSlideNumber >= textures.size()) 
        return 1; 
 
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    fsTexturer.render(textures[currentSlideNumber], slideAlpha); 
    glDisable(GL_BLEND); 

    return 0; 
} 

void 
Slides::update() 
{
    static bool pingpong = true; 
    static float slideDisplayTimer = textureDisplayTime; 

    if(currentSlideNumber >= textures.size()) // rendering is completed if currentSideNumber reaches total texture count 
        return; 

    if(pingpong == true) // increase alpha 
    {
        if(slideAlpha < 1.0) 
            slideAlpha += fadeSpeed/1000.0f; 
        else 
        {
            slideDisplayTimer -= 0.01f; 
            if(slideDisplayTimer <= 0.0) 
            {
                pingpong = false; 
                slideDisplayTimer = textureDisplayTime; 
            } 
        }
        
    } 
    else // decrease alpha 
    {
        if(slideAlpha > 0.0) 
            slideAlpha -= fadeSpeed/1000.0f; 
        else 
        {
            slideDisplayTimer -= 0.01f; 
            if(slideDisplayTimer < 0.0) 
            {
                pingpong = true; 
                slideDisplayTimer = textureDisplayTime; 
                currentSlideNumber += 1; 
            } 
        } 
    } 
} 

void 
Slides::uninitialize() 
{
    logFile.log("---- Slides::uninitialize() started ----\n");

    glDeleteTextures(textures.size(), textures.data());  
    textures.clear(); 

    logFile.log("---- Slides::uninitialize() completed ----\n"); 
}  
