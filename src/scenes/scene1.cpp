#include "../../headers/scenes/scene1.hpp" 

Scene1::Scene1() 
{
    // code  
} 

int Scene1::initialize() 
{	
    // code 
    logFile.log("------------------ Scene1::initialize() started ----------------\n"); 

    lightPosition[0] = 0.286; 
    lightPosition[1] = 3.029; 
    lightPosition[2] = -0.228; 
    lightPosition[3] = 1.0f; 

    if(initGlowingPlanetProgram() == false) 
        logFile.log("Scene1::initialize > glowing-planet program craetion failed\n"); 
    else 
        logFile.log("Scene1::initialize > glowing-planet program created\n"); 

    quad.initialize(); 
    texture_moon = loadTexture("res\\moon.png", FALSE); 

    logFile.log("------------------ Scene1::initialize() completed ----------------\n\n"); 
    return (0); 
} 

bool Scene1::initGlowingPlanetProgram() 
{
    // function declarations 
    void uninitialize(void); 

    // code 
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/glowingPlanet.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/glowingPlanet.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    glowingPlanetProgram.create(shaders, attributes); 

    // get uniform locations 
    timeUniform_glowingPlanet =             glowingPlanetProgram.getUniformLocation("uTime"); 
	resoulutionUniform_glowingPlanet =      glowingPlanetProgram.getUniformLocation("uResolution"); 
	lightPositionUniform_glowingPlanet =    glowingPlanetProgram.getUniformLocation("uLightPosition"); 
	textureUniform_glowingPlanet =          glowingPlanetProgram.getUniformLocation("uTexture"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

void Scene1::display() 
{
    glowingPlanetProgram.use(); 

    glUniform3fv(lightPositionUniform_glowingPlanet, 1, lightPosition); 
    glUniform2fv(resoulutionUniform_glowingPlanet, 1, 
                vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))); 
    glUniform1f(timeUniform_glowingPlanet, mainTimer);
    
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture_moon); 
	glUniform1i(textureUniform_glowingPlanet, 0); 

	quad.render(); 

	glBindTexture(GL_TEXTURE_2D, 0); 

    glowingPlanetProgram.unuse(); 
} 

void Scene1::update() 
{
    // code 
} 

Scene1::~Scene1() 
{
} 

void Scene1Callbacks(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    float cameraStep = 0.5f; 
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

