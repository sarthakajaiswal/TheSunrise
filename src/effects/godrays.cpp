#include "..\..\headers\effects\godrays.hpp" 

Godrays::Godrays()
{
} 

int Godrays::initialize() 
{
    logFile.log("---- Godrays::initialize() ----\n"); 
    assert(initOpenGLState() == 0); 

    quad.initialize(); 

    assert(sceneObjectsFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(lightSourceFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(occlusionFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(sceneFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    logFile.log("---- Godrays::initialize() completed successfully ----\n"); 
    return (0); 
} 

int Godrays::initOpenGLState() 
{
    assert(initOcclusionProgram() == 0); 
    assert(initSceneProgram() == 0); 

    return (0); 
} 

int Godrays::initOcclusionProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    // sihoulette program 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/silhouetteFromObjectsAndLight.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/silhouetteFromObjectsAndLight.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    silhoutteFromObjectsAndLightProgram.create(shaders, attributes); 

    // get uniform locations 
    objectsTextureUniform_silhotte = silhoutteFromObjectsAndLightProgram.getUniformLocation("uObjectsTexture"); 
    lightSourceTextureUniform_silhoutte = silhoutteFromObjectsAndLightProgram.getUniformLocation("uLightTexture"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

int Godrays::initSceneProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    // sihoulette program 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/sceneFromObjectsAndLight.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/sceneFromObjectsAndLight.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    sceneFromObjectsAndLightProgram.create(shaders, attributes); 

    // get uniform locations 
    objectsTextureUniform_scene = sceneFromObjectsAndLightProgram.getUniformLocation("uObjectsTexture"); 
    lightSourceTextureUniform_scene = sceneFromObjectsAndLightProgram.getUniformLocation("uLightTexture"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

GLuint Godrays::getOcclusionTexture() 
{
    occlusionFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        silhoutteFromObjectsAndLightProgram.use(); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getTextureID()); 
        glUniform1i(objectsTextureUniform_silhotte, 0); 

        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getTextureID()); 
        glUniform1i(lightSourceTextureUniform_silhoutte, 1); 

        quad.render(); 
        
        silhoutteFromObjectsAndLightProgram.unuse(); 
    } 
    occlusionFBO.unbind(); 

    return (occlusionFBO.getTextureID()); 
} 

GLuint Godrays::getSceneTexture() 
{
    sceneFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        sceneFromObjectsAndLightProgram.use(); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getTextureID()); 
        glUniform1i(objectsTextureUniform_scene, 0); 

        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getTextureID()); 
        glUniform1i(lightSourceTextureUniform_scene, 1); 

        quad.render(); 
        
        sceneFromObjectsAndLightProgram.unuse(); 
    } 
    sceneFBO.unbind(); 

    return (sceneFBO.getTextureID()); 
} 
