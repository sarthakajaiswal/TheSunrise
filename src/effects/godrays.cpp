#include "..\..\headers\effects\godrays.hpp" 

Godrays::Godrays()
{
} 

int Godrays::initialize() 
{
    logFile.log("---- Godrays::initialize() ----\n"); 
    
    initRadialBlurProgram(); 
    initFinalCompositeProgram(); 
    logFile.log("Godrays::initialize() > shader programs initialized succesfully\n"); 

    assert(quad.initialize() == 0); 

    assert(occlusionFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(godraysFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    logFile.log("Godrays::initialize() > all required FBOs initialized successfully\n"); 

    logFile.log("---- Godrays::initialize() completed successfully ----\n"); 
    return (0); 
} 

void Godrays::renderWithScene(GLuint sceneTexture, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix, float exposure, float decay, float density, float weight, float strength, int numSamples, vmath::vec3 lightPosition) 
{
    vec2 lightPositionOnScreen = convertToScreenSpace(lightPosition, _viewMatrix, _projectionMatrix); 
    GLuint godraysTexture = createRadialBlurTexture(occlusionFBO.getTextureID(), exposure, decay, density, weight, numSamples, vec2(lightPositionOnScreen[0], 1.0-lightPositionOnScreen[1])); 

    finalCompositeProgram.use(); 
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, sceneTexture); 
    glUniform1i(sceneTextureUniform_finalComposite, 0); 
    glActiveTexture(GL_TEXTURE1); 
    glBindTexture(GL_TEXTURE_2D, godraysTexture); 
    glUniform1i(godRaysTextureUniform_finalComposite, 1);

    glUniform1f(godRaysStrengthUniform_finalComposite, strength); 
    
    quad.render(); 
    glBindTexture(GL_TEXTURE_2D, 0); 

    finalCompositeProgram.unuse();
} 

void Godrays::uninitialize() 
{
    logFile.log("Godrays::uninitialize() Uninitializing...\n"); 
    occlusionFBO.destroyFBO(); 
    logFile.log("Godrays::uninitialize() Uninitialization completed\n"); 
} 

int Godrays::initRadialBlurProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    // sihoulette program 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/radialBlur.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/Godrays/radialBlur.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    radialBlurProgram.create(shaders, attributes); 

    // get uniform locations 
    exposureUniform_radialBlur = radialBlurProgram.getUniformLocation("uExposure"); 
    decayUniform_radialBlur = radialBlurProgram.getUniformLocation("uDecay"); 
    densityUniform_radialBlur = radialBlurProgram.getUniformLocation("uDensity"); 
    weightUniform_radialBlur = radialBlurProgram.getUniformLocation("uWeight"); 
    lightPositionOnScreenUniform_radialBlur = radialBlurProgram.getUniformLocation("uLightPositionOnScreen"); 
    occlusionTextureUniform_radialBlur = radialBlurProgram.getUniformLocation("uOcclusionTextureSampler"); 
    numSamplesUniform_radialBlur = radialBlurProgram.getUniformLocation("uNumSamples"); 

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

GLuint Godrays::createRadialBlurTexture(GLuint occlusionTexture, float exposure, float decay, float density, float weight, int numSamples, vmath::vec2 lightPositionOnScreen) 
{
    godraysFBO.bind(); 
    {
        glClearColor(0.0, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        radialBlurProgram.use(); 

        glUniform1f(exposureUniform_radialBlur, exposure);
        glUniform1f(decayUniform_radialBlur, decay);
        glUniform1f(densityUniform_radialBlur, density);
        glUniform1f(weightUniform_radialBlur, weight);
        glUniform2fv(lightPositionOnScreenUniform_radialBlur, 1, lightPositionOnScreen);
        glUniform1i(numSamplesUniform_radialBlur, numSamples); 
        
        glActiveTexture(GL_TEXTURE0); 
        glUniform1i(occlusionTextureUniform_radialBlur, 0); 
        glBindTexture(GL_TEXTURE_2D, occlusionTexture); 
        
        quad.render(); 
        
        radialBlurProgram.unuse(); 
    } 
    godraysFBO.unbind(); 

    return (godraysFBO.getTextureID()); 
} 

// // combines scene texture and occlusion/silhoutte texture 
// GLuint Godrays::getFinalCompositeTexture() 
// {
//     finalCompositeFBO.bind(); 
//     {
//         glClearColor(0.0, 0.0, 0.0, 1.0); 
//         glClear(GL_COLOR_BUFFER_BIT); 

//         finalCompositeProgram.use(); 

//         glActiveTexture(GL_TEXTURE0); 
//         glBindTexture(GL_TEXTURE_2D, sceneFBO.getTextureID()); 
//         glUniform1i(sceneTextureUniform_finalComposite, 0); 

//         glActiveTexture(GL_TEXTURE1); 
//         glBindTexture(GL_TEXTURE_2D, radialBlurFBO.getTextureID()); 
//         glUniform1i(godRaysTextureUniform_finalComposite, 1);

//         glUniform1f(godRaysStrengthUniform_finalComposite, 1.0f); 
        
//         quad.render(); 
        
//         glBindTexture(GL_TEXTURE_2D, 0); 
//         finalCompositeProgram.unuse(); 
//     } 
//     finalCompositeFBO.unbind(); 

//     return (finalCompositeFBO.getTextureID()); 
// } 

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

