#include "..\..\headers\effects\godrays.hpp" 

Godrays::Godrays()
{
} 

int Godrays::initialize() 
{
    logFile.log("---- Godrays::initialize() ----\n"); 
    assert(initOpenGLState() == 0); 

    assert(sceneObjectsFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(lightSourceFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    logFile.log("---- Godrays::initialize() completed successfully ----\n"); 
    return (0); 
} 

int Godrays::initOpenGLState() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/occlusion.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/occlusion.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    occlusionProgram.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform_occlusion = occlusionProgram.getUniformLocation("uMVPMatrix"); 
    isLightUniform_occlusion = occlusionProgram.getUniformLocation("uIsLight"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

