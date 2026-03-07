#include "../../headers/scenes/introScene.hpp" 

Camera introSceneCamera; 

extern mat4 projectionMatrix; 
extern void resize(int, int); 

IntroScene::IntroScene() 
{
    // code 
} 

int IntroScene::initialize() 
{
    // code 
    logFile.log("------------------ IntroScene::initialize() started ----------------\n"); 

    if(initHeadingAlphabetsShaderProgram() == false) 
        logFile.log("IntroScene::initialize > heading-alphabet program craetion failed\n"); 
    else 
        logFile.log("IntroScene::initialize > heading-alphabet program created\n"); 

    if(initBrightColorSeparatorProgram() == false) 
        logFile.log("IntroScene::initialize > Bright-Color-separator program craetion failed\n"); 
    else 
        logFile.log("IntroScene::initialize > Bright-Color-separator program created\n"); 

    // FBOs 
	assert(fbo_scene.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	assert(fbo_brightColors.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	logFile.log("IntroScene::initialize > required FBOs created\n"); 

    // effects  
    blueEffect.initialize(); 
    blendTextureEffect.initialize(); 

    quad.initialize(); 
    logFile.log("IntroScene::initialize > initializing alphabets...\n"); 
    initAlphabet_A(); 
    initAlphabet_S(); 
    initAlphabet_T(); 
    initAlphabet_R(); 
    initAlphabet_O(); 
    initAlphabet_M(); 
    initAlphabet_E(); 
    initAlphabet_D(); 
    initAlphabet_I(); 
    initAlphabet_C(); 
    initAlphabet_P(); 
    logFile.log("IntroScene::initialize > Alphabets initialized\n"); 

    texture_marbleColor = loadTexture("res\\BlackMarble.png", FALSE); 
    texture_marbleNormalMap = loadTexture("res\\BlackMarbleNormalMap.png", FALSE); 

    logFile.log("------------------ IntroScene::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void IntroScene::display() 
{
    // code 
    static bool isFirstCall = true; 
    if(isFirstCall == true) 
    {
        localTimer = 0.0f; 
        isFirstCall = false; 
    } 

    // render scene to fbo 
    fbo_scene.bind(); 
    {
        headingAlphabetsShaderProgram.use(); 
        mat4 modelMatrix = mat4::identity(); 
        viewMatrix = introSceneCamera.getViewMatrix(CAMERA_GAME_MODE); 

        glUniformMatrix4fv(projectionMatrixUniform_heading, 1, GL_FALSE, projectionMatrix);   
        glUniform1f(blendStrengthUniform_heading, blendStrength); 
        glUniform4fv(lightPositionUniform_heading, 1, lightPosition);  
        glUniform3fv(laUniform_heading, 1, lightAmbient);  
        glUniform3fv(ldUniform_heading, 1, lightDiffuse);  
        glUniform3fv(lsUniform_heading, 1, lightSpecular);  
        glUniform1f(materialShininessUniform_heading, materialShininess); 
        glUniform3fv(kaUniform_heading, 1, materialAmbient);  
        glUniform3fv(kdUniform_heading, 1, materialDiffuse);  
        glUniform3fv(ksUniform_heading, 1, materialSpecular);  
    
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, texture_marbleNormalMap); 
        glUniform1i(normalMapTextureSamplerUniform_heading, 0); 

        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, texture_marbleColor); 
        glUniform1i(colorTextureSamplerUniform_heading, 1); 

        mat4 modelViewMatrix = mat4::identity(); 
        
        // if(mainTimer < 15.0f) 
        // {
            vec3 astromedicompStartingLocation = {astrmomedicompStartingX, 0.0f, astromedicompZ}; 
            modelMatrix = vmath::translate(astromedicompStartingLocation[0], astromedicompStartingLocation[1], astromedicompStartingLocation[2]); 
            modelMatrix = modelMatrix * vmath::scale(alphabetSx, alphabetSy, alphabetSz); 

            modelViewMatrix = viewMatrix * modelMatrix; 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_A();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_S();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_T();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_R();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_O();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_M();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_E();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_I();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_C();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_O();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_M();    
            modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
            glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
            renderAlphabet_P();    
        // } 
        // else 
        // {
        //     static bool isFirstTime = true; 
        //     if(isFirstTime == true) 
        //     {
        //         vec3 cpaStartingLocation = {-10.0, 0.0f, astromedicompZ}; 
        //         modelMatrix = vmath::translate(cpaStartingLocation[0], cpaStartingLocation[1], cpaStartingLocation[2]); 
        //         modelMatrix = modelMatrix * vmath::scale(alphabetSx, alphabetSy, alphabetSz); 

        //         isFirstTime = false; 
        //     } 

        //     modelViewMatrix = viewMatrix * modelMatrix; 
        //     glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
        //     renderAlphabet_C();    
        //     modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
        //     glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
        //     renderAlphabet_P();    
        //     modelViewMatrix = modelViewMatrix * vmath::translate(alphabetSpacing, 0.0f, 0.0f); 
        //     glUniformMatrix4fv(modelViewMatrixUniform_heading, 1, GL_FALSE, modelViewMatrix); 
        //     renderAlphabet_A();  
        // } 

        // =========================== ORIGINAL SCENE END =========================== 

        headingAlphabetsShaderProgram.unuse(); 
    } 
    fbo_scene.unbind(); 

    // extract bright colors from scene 
    fbo_brightColors.bind(); 
    {
        brightColorSeparatorProgram.use();  
        glActiveTexture(GL_TEXTURE0); 
        glBindTexture(GL_TEXTURE_2D, fbo_scene.getTextureID()); 
        glUniform1i(textureUniform_brightColors, 0); 
        quad.render(); 
    } 
	fbo_brightColors.unbind(); 

	// applying blueEffect on bright colors 
    blurTexture = blueEffect.render(fbo_brightColors.getTextureID(), blurIterations); 

    // combining blur and scene texture
    blendedTexture = blendTextureEffect.render(fbo_scene.getTextureID(), 1.0, blurTexture, 1.0); 
    
    // Finally, render blended texture on screen  
    fsTexture.render(blendedTexture); 

	// ************** CHECK FBO TEXTURES *************** 
	// glViewport(1000, 400, 480, 270);
    // fsTextureProgram.use(); 
	// glActiveTexture(GL_TEXTURE0); 
	// glBindTexture(GL_TEXTURE_2D, blurTexture); 
	// glUniform1i(textureUniform_fsTexture, 0);  
	// quad.render(); 
	// glBindTexture(GL_TEXTURE_2D, 0); 
	// glUseProgram(0);
} 

void IntroScene::update() 
{
    // code  
    static float headingGoingBackStep = 0.95f; 

    if(mainTimer < 15.0)
    {
        if(astromedicompZ > -50.0) 
        {
            astromedicompZ -= headingGoingBackStep; 
            if(headingGoingBackStep > 0.008) 
                headingGoingBackStep -= 0.009f; 
        } 

        if(mainTimer > 10.5 && blurIterations < 15 && blendStrength < 3.00) 
        {
            blurIterations += 1; 
            blendStrength += 0.1f; 
        } 
    } 
} 

void IntroScene::uninitialize() 
{

} 

void IntroScene::eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    introSceneCamera.cameraCallback(hwnd, uMsg, wParam, lParam); 
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

IntroScene::~IntroScene() 
{
    // code 
} 

bool IntroScene::initHeadingAlphabetsShaderProgram() 
{
    // function declarations 
    void uninitialize(void); 

    // code 
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/introAlphabets.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/introAlphabets.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_COLOR, "aColor")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_NORMAL, "aNormal")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TANGENT, "aTangent")); 
	
    headingAlphabetsShaderProgram.create(shaders, attributes); 

    // get uniform locations 
    modelViewMatrixUniform_heading =    headingAlphabetsShaderProgram.getUniformLocation("uModelViewMatrix"); 
	projectionMatrixUniform_heading =   headingAlphabetsShaderProgram.getUniformLocation("uProjectionMatrix"); 
    // lighting 
	lightPositionUniform_heading =      headingAlphabetsShaderProgram.getUniformLocation("uLightPosition"); 
	laUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uLightAmbient"); 
	ldUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uLightDiffuse"); 
	lsUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uLightSpecular"); 
	materialShininessUniform_heading =  headingAlphabetsShaderProgram.getUniformLocation("uMaterialShininess"); 
	kaUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uMaterialAmbient"); 
	kdUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uMaterialDiffuse"); 
	ksUniform_heading =                 headingAlphabetsShaderProgram.getUniformLocation("uMaterialSpecular"); 
	// normal mapping
	normalMapTextureSamplerUniform_heading =    headingAlphabetsShaderProgram.getUniformLocation("uNormalMapSampler"); 
	colorTextureSamplerUniform_heading =        headingAlphabetsShaderProgram.getUniformLocation("uColorTextureSampler"); 
	// normalMapEnableUniform_heading =            headingAlphabetsShaderProgram.getUniformLocation("uIsNormalMapEnabled"); 
    // blending 
	blendStrengthUniform_heading =  headingAlphabetsShaderProgram.getUniformLocation("uBlendStrength"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

bool IntroScene::initBrightColorSeparatorProgram()
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/brightColorSeparator.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/brightColorSeparator.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    brightColorSeparatorProgram.create(shaders, attributes); 

    // get uniform locations 
    textureUniform_brightColors = brightColorSeparatorProgram.getUniformLocation("uTexture"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 



