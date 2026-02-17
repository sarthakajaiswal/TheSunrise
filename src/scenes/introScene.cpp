#include "../../headers/scenes/introScene.hpp" 

// Camera scene1Camera; 
GLfloat cameraPosX = 0.0f; 
GLfloat cameraPosY = 0.0f; 
GLfloat cameraPosZ = 20.0f; 
GLfloat cameraCenterX = 0.0f; 
GLfloat cameraCenterY = 0.0f; 
GLfloat cameraCenterZ = -30.0f; 
GLfloat cameraRotateAngle = 0.0f; 
GLfloat cameraRadius = 0.0f; 

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
    
    if(initGuassianBlurProgram() == false) 
        logFile.log("IntroScene::initialize > guassian-blur program craetion failed\n"); 
    else 
        logFile.log("IntroScene::initialize > guassian-blur program created\n"); 
    
    if(initTextureBlendProgram() == false) 
        logFile.log("IntroScene::initialize > texture-blend program craetion failed\n"); 
    else 
        logFile.log("IntroScene::initialize > texture-blend program created\n"); 
    
    if(initializeFullScreenTextureProgram() == false) 
        logFile.log("IntroScene::initialize > fullscreen-texture program craetion failed\n"); 
    else 
        logFile.log("IntroScene::initialize > fullscreen-texture program created\n"); 

    // FBOs 
	assert(fbo_scene.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	assert(fbo_brightColors.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	assert(fbos_guassianBlur[0].createFBOWithoutDepthBuffer(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	assert(fbos_guassianBlur[1].createFBOWithoutDepthBuffer(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
    logFile.log("IntroScene::initialize > required FBOs created\n"); 

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
    // ***************************** PASS 1: SCENE TO FLOATING PT FBO ********************************************** 
    fbo_scene.bind(); 
    headingAlphabetsShaderProgram.use(); 

    // =========================== ORIGINAL SCENE START =========================== 

	cameraPosX = cameraRadius * sin(cameraRotateAngle * 3.14/180.0); 
	cameraPosZ = cameraRadius * cos(cameraRotateAngle * 3.14/180.0); 
	viewMatrix = vmath::lookat(
		vec3(cameraPosX, cameraPosY, cameraPosZ), 
		vec3(cameraCenterX, cameraCenterY, cameraCenterZ), 
		vec3(0.0, 1.0, 0.0)
	);  

    mat4 modelMatrix = mat4::identity(); 
    vec3 headingStartingLocation = {headingStartingX, 0.0f, headingZ}; 
    modelMatrix = vmath::translate(headingStartingLocation[0], headingStartingLocation[1], headingStartingLocation[2]); 
    modelMatrix = modelMatrix * vmath::scale(alphabetSx, alphabetSy, alphabetSz); 

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

    mat4 modelViewMatrix = viewMatrix * modelMatrix; 
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

    // =========================== ORIGINAL SCENE END =========================== 

    headingAlphabetsShaderProgram.unuse(); 
    fbo_scene.unbind(); 

    // ***************************** EXTRACTING BRIGHT COLORS FROM SCENE TEXTURE INTO FBO_BRIGHTCOLORS ********************************************** 
    fbo_brightColors.bind(); 

	brightColorSeparatorProgram.use();  

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, fbo_scene.getTextureID()); 
	glUniform1i(textureUniform_brightColors, 0); 

	quad.render(); 

	glBindFramebuffer(GL_FRAMEBUFFER, 0); 

    // ***************************** APPYING BLUR ON BRIGHT COLORS ********************************************** 
	bool fboPingPong = false; 
	for(unsigned int i = 0; i < blurIterations; ++i) 
	{
		FBO fboToBeBound; 
		GLuint inputTexture; 
		bool horizontalOrVertical = 0; 
		
		if(fboPingPong == false) 
		{
			fboToBeBound = fbos_guassianBlur[0]; 
			inputTexture = fbos_guassianBlur[1].getTextureID(); 
			horizontalOrVertical = 0; 
		} 
		else 
		{
			fboToBeBound = fbos_guassianBlur[1]; 
			inputTexture = fbos_guassianBlur[0].getTextureID(); 
			horizontalOrVertical = 1; 
		} 

		if(i==0) 
			inputTexture = fbo_brightColors.getTextureID(); 
		
		guassianBlurProgram.use(); 
		fboToBeBound.bind(); 

		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, inputTexture); 
		glUniform1i(textureSamplerUniform_guassianBlur, 0); 
		glUniform1i(horizontalOrVerticalUniform_guassianBlur, horizontalOrVertical); 
		quad.render(); 
		glUseProgram(0); 

		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		fboPingPong = !fboPingPong; 
	} 

    // *********************** COMBINING BLUR AND SCENE TEXTURE : TO SCREEN *********************************** 
	resize(winWidth, winHeight); 
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

    blendTextureProgram.use();  

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, fbo_scene.getTextureID()); 
	glUniform1i(texture1Uniform_blendProgram, 0);

	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, fbos_guassianBlur[0].getTextureID()); 
	glUniform1i(texture2Uniform_blendProgram, 1); 

	quad.render(); 

	glBindTexture(GL_TEXTURE_2D, 0); 
	glUseProgram(0); 

    // ************** CHECK FBO TEXTURES *************** 
	// // scene 
	// glViewport(10, 600, 300, 180); 
	// fsTextureProgram.use();
	// glActiveTexture(GL_TEXTURE0); 
	// glBindTexture(GL_TEXTURE_2D, fbo_scene.getTextureID()); 
	// glUniform1i(textureUniform_fsTexture, 0);  
	// quad.render();  
	// glBindTexture(GL_TEXTURE_2D, 0); 
	// fsTextureProgram.unuse(); 

	// // bright colors  
	// glViewport(1000, 400, 480, 270);
    // fsTextureProgram.use(); 
	// glActiveTexture(GL_TEXTURE0); 
	// glBindTexture(GL_TEXTURE_2D, fbo_brightColors.getTextureID()); 
	// glUniform1i(textureUniform_fsTexture, 0);  
	// quad.render(); 
	// glBindTexture(GL_TEXTURE_2D, 0); 
	// glUseProgram(0);
} 

void IntroScene::update() 
{
    // code  
    if(mainTimer > 3.0 && headingZ < -50.0) 
    {} 
} 

void IntroScene::uninitialize() 
{

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

bool IntroScene::initGuassianBlurProgram()
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/guassianBlur.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/guassianBlur.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    guassianBlurProgram.create(shaders, attributes); 

    // get uniform locations 
    textureSamplerUniform_guassianBlur = guassianBlurProgram.getUniformLocation("uTextureSampler"); 
    horizontalOrVerticalUniform_guassianBlur = guassianBlurProgram.getUniformLocation("uHorizontalOrVertical"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

bool IntroScene::initTextureBlendProgram()
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/textureBlend.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/textureBlend.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    blendTextureProgram.create(shaders, attributes); 

    // get uniform locations 
    texture1Uniform_blendProgram = blendTextureProgram.getUniformLocation("uTexture1"); 
    texture2Uniform_blendProgram = blendTextureProgram.getUniformLocation("uTexture2"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

bool IntroScene::initializeFullScreenTextureProgram()
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/fullScreenTexture.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/fullScreenTexture.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    fsTextureProgram.create(shaders, attributes); 

    // get uniform locations 
    textureUniform_fsTexture = fsTextureProgram.getUniformLocation("uTexture"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 



