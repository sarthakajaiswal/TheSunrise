#include "..\..\headers\effects\water.hpp" 

Water::Water() 
{
    deltaTime = 0.0f; 
} 

int Water::initialize() 
{
    logFile.log("---- Water::initialize() started --\n"); 

    int result = initOpenGLState(); 
    // TODO: 
    // if(initOpenGLState() != 0) 
    //     throw opengl_initialization_failure("Terrain::initialize() > InitOpenGLState() failed\n");
    if(result == 0) 
		logFile.log("Water::initialize() > opengl state initialized\n"); 

    assert(initBWShader() == 0); 

    assert(reflectionFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
    assert(refractionFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
    logFile.log("Water::initialize() > fbos created succesfully\n"); 

    assert(waterRect.initialize() == 0); 
    logFile.log("Water::initialize() > water rect initialized succesfully\n"); 

    // textures 
    dudvTexture = loadTexture("res/waterDUDV.png", FALSE); 
    normalMap = loadTexture("res/waterNormalMap.png", FALSE); 
 
	QueryPerformanceFrequency(&this->freq); 
	one_divided_by_freq = 1.0f / (float)this->freq.QuadPart; 
	QueryPerformanceCounter(&this->startTime); 

    logFile.log("---- Water::initialize() completed --\n"); 
    return 0; 
} 

int Water::initOpenGLState()
{
    // variable declarations 
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
 
    // code 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/water.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/water.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
	
    shaderProgram.create(shaders, attributes); 

    // get uniform locations 
    modelMatrixUniform = shaderProgram.getUniformLocation("uModelMatrix"); 
    viewMatrixUniform = shaderProgram.getUniformLocation("uViewMatrix"); 
    projectionMatrixUniform = shaderProgram.getUniformLocation("uProjectionMatrix");  

    reflectionTextureUniform = shaderProgram.getUniformLocation("uReflectionTexture"); 
    refractionTextureUniform = shaderProgram.getUniformLocation("uRefractionTexture"); 
    dudvTextureUniform = shaderProgram.getUniformLocation("uDuDvTexture"); 
    normalMapUniform = shaderProgram.getUniformLocation("uNormalMap"); 
    depthMapUniform = shaderProgram.getUniformLocation("uDepthMap");  
    screenSizeUniform = shaderProgram.getUniformLocation("uScreenSize"); 
    moveFactorUniform = shaderProgram.getUniformLocation("uMoveFactor"); 
    cameraPositionUniform = shaderProgram.getUniformLocation("uCameraPosition"); 
    lightPositionUniform = shaderProgram.getUniformLocation("uLightPosition"); 
    lightColorUniform = shaderProgram.getUniformLocation("uLightColor");  

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    return (0); 
} 

void Water::render(mat4 modelMatrix, mat4 viewMatrix, mat4 projectionMatrix, vec3 cameraPosition, vec3 lightPosition, float waveSpeed) 
{
	// variable declarations 
	static float moveFactor = 0.0f; 

	// code 
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	shaderProgram.use(); 
	
	glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, modelMatrix); 
	glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, viewMatrix); 
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, projectionMatrix); 

	glUniform2fv(screenSizeUniform, 1, vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN))); 
	glUniform3fv(cameraPositionUniform, 1, cameraPosition); 
	glUniform3fv(lightPositionUniform, 1, lightPosition); 
	glUniform3fv(lightColorUniform, 1, vec3(1.0, 1.0, 1.0)); 

	moveFactor += waveSpeed * deltaTime; 
	moveFactor = fmod(moveFactor, 1.0); 
	glUniform1f(moveFactorUniform, moveFactor);  
	
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, reflectionFBO.getTextureID()); 
	glUniform1i(reflectionTextureUniform, 0); 

	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, refractionFBO.getTextureID()); 
	glUniform1i(refractionTextureUniform, 1); 

	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, dudvTexture); 
	glUniform1i(dudvTextureUniform, 2); 

	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, normalMap); 
	glUniform1i(normalMapUniform, 3); 

	// glActiveTexture(GL_TEXTURE4); 
	// glBindTexture(GL_TEXTURE_2D, refractionRBO);
	// glUniform1i(depthMapUniform, 4); 

    waterRect.render(); 

	glBindTexture(GL_TEXTURE_2D, 0); 

    shaderProgram.unuse(); 

    glDisable(GL_BLEND); 

    // bwShader.use(); 
    // glUniformMatrix4fv(mvpMatrixUniform_bwShader, 1, GL_FALSE, projectionMatrix*viewMatrix*modelMatrix); 
    // waterRect.render(); 
    // bwShader.unuse(); 
} 

void Water::update() 
{
    QueryPerformanceCounter(&currentTime); 
    this->deltaTime = (float)(this->currentTime.QuadPart - this->startTime.QuadPart)*this->one_divided_by_freq; 
    this->startTime = this->currentTime; 
} 

void Water::uninitialize() 
{
    // code 
    logFile.log("Water::uninitialize() uninitializing Water...\n"); 

    if(normalMap != 0) 
    {
        glDeleteTextures(1, &normalMap); 
        normalMap = 0; 
    } 
    if(dudvTexture != 0) 
    {
        glDeleteTextures(1, &dudvTexture); 
        dudvTexture = 0; 
    } 

    refractionFBO.destroyFBO(); 
    reflectionFBO.destroyFBO(); 

    waterRect.uninitialize();  
    logFile.log("Water::uninitialize() Water uniniialized\n"); 
} 

Water::~Water() 
{
} 

Water::WaterRect::WaterRect() : vao(0), vbo(0) 
{
} 

int Water::WaterRect::initialize() 
{
		const GLfloat quad_PCNT[] =
	{						 
		// position				// color			 // normals				// texcoords
		1.0f,  0.0f, -1.0f,		0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 1.0f,
		-1.0f,  0.0f, -1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 1.0f,
		-1.0f,  0.0f,  1.0f,	0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	0.0f, 0.0f,
		1.0f,  0.0f,  1.0f,		0.0f, 1.0f, 0.0f,	 0.0f,  1.0f,  0.0f,	1.0f, 0.0f,
	};

	// quad 
	glGenVertexArrays(1, &vao); // vao-vertex array object 
	glBindVertexArray(vao); 
	
	// create a common vbo for p, c, n, t 
	glGenBuffers(1, &vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, vbo); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_PCNT), quad_PCNT, GL_STATIC_DRAW); 

	// for position 
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(0 * sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
	
	// for color 
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(3 * sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR); 

	// for normals 
	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(6 * sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL); 

	// for texCoords 
	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 11*sizeof(float), (void*)(9*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD); 

	glBindVertexArray(0); 

    return 0; 
} 

void Water::WaterRect::render() 
{
    glBindVertexArray(vao); 
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6); 
	glBindVertexArray(0); 
} 

void Water::WaterRect::uninitialize() 
{
    if(vbo) 
    {
        glDeleteBuffers(1, &vbo); 
        vbo = 0; 
    } 
    if(vao) 
    {
        glDeleteVertexArrays(1, &vao); 
        vao = 0; 
    } 
} 


int Water::initBWShader() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/bw.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/bw.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    
    bwShader.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform_bwShader = bwShader.getUniformLocation("uMVPMatrix"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 
