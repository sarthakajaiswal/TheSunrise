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
    assert(motionBlurFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    logFile.log("---- Godrays::initialize() completed successfully ----\n"); 
    return (0); 
} 

int Godrays::initOpenGLState() 
{
    assert(initOcclusionProgram() == 0); 
    assert(initSceneProgram() == 0); 
    assert(initMotionBlurProgram() == 0); 

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

int Godrays::initMotionBlurProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    // sihoulette program 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/motionBlur.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/motionBlur.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    motionBlurProgram.create(shaders, attributes); 

    // get uniform locations 
    exposureUniform_motionBlur = motionBlurProgram.getUniformLocation("uExposure"); 
    decayUniform_motionBlur = motionBlurProgram.getUniformLocation("uDecay"); 
    densityUniform_motionBlur = motionBlurProgram.getUniformLocation("uDensity"); 
    weightUniform_motionBlur = motionBlurProgram.getUniformLocation("uWeight"); 
    lightPositionOnScreenUniform_motionBlur = motionBlurProgram.getUniformLocation("uLightPositionOnScreen"); 
    occlusionTextureUniform_motionBlur = motionBlurProgram.getUniformLocation("uOcclusionTextureSampler"); 
    numSamplesUniform_motionBlur = motionBlurProgram.getUniformLocation("uNumSamples"); 

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

GLuint Godrays::getMotionBlurTexture() 
{
    float gExposure = 0.340; 
    float gDecay = 0.960; 
    float gDensity = 0.840; 
    float gWeight =  0.580; 
    vmath::vec2 lightPositionOnScreen = vmath::vec2(0.5, 0.5); 
    float gNumSamples = 100; 

    motionBlurFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        motionBlurProgram.use(); 

        glUniform1f(exposureUniform_motionBlur, gExposure);
        glUniform1f(decayUniform_motionBlur, gDecay);
        glUniform1f(densityUniform_motionBlur, gDensity);
        glUniform1f(weightUniform_motionBlur, gWeight);
        // if (gAutoLightPos) {
            glUniform2fv(lightPositionOnScreenUniform_motionBlur, 1, lightPositionOnScreen);
        // } else {
        //     glUniform2fv(lightPositionOnScreenUniform_motionBlur, 1, gManualLightPos);
        // }
        glUniform1i(numSamplesUniform_motionBlur, gNumSamples); 
        
        glActiveTexture(GL_TEXTURE0); 
        glUniform1i(occlusionTextureUniform_motionBlur, 0); 
        glBindTexture(GL_TEXTURE_2D, occlusionFBO.getTextureID()); 
        
        quad.render(); 
        
        motionBlurProgram.unuse(); 
    } 
    motionBlurFBO.unbind(); 

    return (motionBlurFBO.getTextureID()); 
} 
