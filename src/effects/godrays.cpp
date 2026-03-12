#include "..\..\headers\effects\godrays.hpp" 

Godrays::Godrays()
{
} 

int Godrays::initialize() 
{
    logFile.log("---- Godrays::initialize() ----\n"); 
    
    assert(initOpenGLState() == 0); 
    logFile.log("Godrays::initialize()::initOpenGLStart() > completed.\n"); 

    assert(quad.initialize() == 0); 

    assert(sceneObjectsFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(lightSourceFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(occlusionFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(sceneFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(motionBlurFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(finalCompositeFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    logFile.log("Godrays::initialize() > all required FBOs initialized successfully\n"); 

    logFile.log("---- Godrays::initialize() completed successfully ----\n"); 
    return (0); 
} 

int Godrays::initOpenGLState() 
{
    assert(initOcclusionProgram() == 0); 
    assert(initSceneProgram() == 0); 
    assert(initMotionBlurProgram() == 0); 
    assert(initFinalCompositeProgram() == 0); 

    assert(fsTexturer.initialize() == 0); 

    return (0); 
} 

// bug: this sets the viewport to complete fullscreen 
// you have to reset the viewport if using smaller after call to this function 
GLuint Godrays::render(vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix, float exposure, float decay, float density, float weight, int numSamples, vmath::vec3 lightPosition) 
{
    createOcclusionTexture(); 
    createSceneTexture(); 

    vec2 lightPositionOnScreen = convertToScreenSpace(lightPosition, _viewMatrix, _projectionMatrix); 
    logFile.log("lightPositionOnScreen = %.2f, %.2f\n", lightPositionOnScreen[0], lightPositionOnScreen[1]); 
    createMotionBlurTexture(exposure, decay, density, weight, numSamples, vec2(lightPositionOnScreen[0], 1.0-lightPositionOnScreen[1])); 

    GLuint finalCompositeTexture = getFinalCompositeTexture(); 

    fsTexturer.render(finalCompositeTexture); 
    return (finalCompositeTexture); 
} 

void Godrays::uninitialize() 
{
    logFile.log("Godrays::uninitialize() Uninitializing...\n"); 
    finalCompositeFBO.destroyFBO(); 
    motionBlurFBO.destroyFBO(); 
    sceneFBO.destroyFBO(); 
    occlusionFBO.destroyFBO(); 
    lightSourceFBO.destroyFBO(); 
    sceneObjectsFBO.destroyFBO(); 
    logFile.log("Godrays::uninitialize() Uninitialization completed\n"); 
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

int Godrays::initFinalCompositeProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    // sihoulette program 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/finalComposite.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/finalComposite.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    finalCompositeProgram.create(shaders, attributes); 

    // get uniform locations 
    sceneTextureUniform_finalComposite = finalCompositeProgram.getUniformLocation("uSceneTexture"); 
    godRaysTextureUniform_finalComposite = finalCompositeProgram.getUniformLocation("uGodRaysTexture"); 
    godRaysStrengthUniform_finalComposite = finalCompositeProgram.getUniformLocation("uGodRaysStrength"); 
    
    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

// creates occlusion texture from objectsFBO and lightSourceFBO (initiated by user) 
GLuint Godrays::createOcclusionTexture() 
{
    occlusionFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        silhoutteFromObjectsAndLightProgram.use(); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getTextureID()); 
        // glUniform1i(objectsTextureUniform_silhotte, 0); 

        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getDepthID()); 

        glActiveTexture(GL_TEXTURE2); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getTextureID()); 
        // glUniform1i(lightSourceTextureUniform_silhoutte, 1); 

        glActiveTexture(GL_TEXTURE3); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getDepthID()); 

        quad.render(); 
        
        silhoutteFromObjectsAndLightProgram.unuse(); 
    } 
    occlusionFBO.unbind(); 

    return (occlusionFBO.getTextureID()); 
} 

// creates scene texture from objectsFBO and lightSourceFBO 
GLuint Godrays::createSceneTexture() 
{
    sceneFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        sceneFromObjectsAndLightProgram.use(); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getTextureID()); 
        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, sceneObjectsFBO.getDepthID()); 
        // glUniform1i(objectsTextureUniform_scene, 0); 

        glActiveTexture(GL_TEXTURE2); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getTextureID()); 
        glActiveTexture(GL_TEXTURE3); 
        glBindTexture(GL_TEXTURE_2D, lightSourceFBO.getDepthID()); 
        // glUniform1i(lightSourceTextureUniform_scene, 1); 

        quad.render(); 
        
        sceneFromObjectsAndLightProgram.unuse(); 
    } 
    sceneFBO.unbind(); 

    return (sceneFBO.getTextureID()); 
} 

// applies motion blur on occlusion/silhoutte texture 
GLuint Godrays::createMotionBlurTexture(float exposure, float decay, float density, float weight, int numSamples, vmath::vec2 lightPositionOnScreen) 
{
    motionBlurFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        motionBlurProgram.use(); 

        glUniform1f(exposureUniform_motionBlur, exposure);
        glUniform1f(decayUniform_motionBlur, decay);
        glUniform1f(densityUniform_motionBlur, density);
        glUniform1f(weightUniform_motionBlur, weight);
        glUniform2fv(lightPositionOnScreenUniform_motionBlur, 1, lightPositionOnScreen);
        glUniform1i(numSamplesUniform_motionBlur, numSamples); 
        
        glActiveTexture(GL_TEXTURE0); 
        glUniform1i(occlusionTextureUniform_motionBlur, 0); 
        glBindTexture(GL_TEXTURE_2D, occlusionFBO.getTextureID()); 
        
        quad.render(); 
        
        motionBlurProgram.unuse(); 
    } 
    motionBlurFBO.unbind(); 

    return (motionBlurFBO.getTextureID()); 
} 

// combines scene texture and occlusion/silhoutte texture 
GLuint Godrays::getFinalCompositeTexture() 
{
    finalCompositeFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        finalCompositeProgram.use(); 

        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, sceneFBO.getTextureID()); 
        glUniform1i(sceneTextureUniform_finalComposite, 0); 

        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, motionBlurFBO.getTextureID()); 
        glUniform1i(godRaysTextureUniform_finalComposite, 1);

        glUniform1f(godRaysStrengthUniform_finalComposite, 1.0f); 
        
        quad.render(); 
        
        glBindTexture(GL_TEXTURE_2D, 0); 
        finalCompositeProgram.unuse(); 
    } 
    finalCompositeFBO.unbind(); 

    return (finalCompositeFBO.getTextureID()); 
} 

/* Helper routines */ 
vmath::mat4 transpose(const vmath::mat4 &m) 
{ 
	vmath::mat4 t; 
	
	for (int i = 0; i < 4; ++i) 
	{ 
		for (int j = 0; j < 4; ++j) 
		{ 
			t[i][j] = m[j][i]; 
		} 
	} 
	return t; 
}

vec2 convertToScreenSpace(vec3 worldPos, mat4 viewMatrix, mat4 projectionMatrix) 
{
	// variable declarations 
	vec2 screenPos; 

	// Step 1: world -> view 
	vec4 viewPos = vec4(worldPos, 1.0) * transpose(viewMatrix); 
	// Step 2: view -> clip 
	vec4 clipPos = viewPos * transpose(projectionMatrix); 

	if(clipPos[3] <= 0.0f) 
		return (vec2(10.0, 10.0)); // position out of screen 
	
	// Step 3: perspective divide 
	clipPos[0] /= clipPos[3]; 
	clipPos[1] /= clipPos[3]; 
	clipPos[2] /= clipPos[3]; 

	// Step 4: NDC [-1,1] -> screen [0,1] 
	screenPos[0] = (clipPos[0] + 1.0f) * 0.5f; 
	screenPos[1] = 1.0f - (clipPos[1] + 1.0f) * 0.5f; // flip Y so top-left is (0,0)	

	return (screenPos); 
}

