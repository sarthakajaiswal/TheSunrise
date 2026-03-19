#include "..\..\headers\effects\guassianBlur.hpp" 

GuassianBlur::GuassianBlur() 
{
	isInitialized = false; 
} 

int 
GuassianBlur::initialize() 
{
    logFile.log("------ GuassianBlur::initialize() started ----\n"); 

    int result = initOpenGLState(); 
    // TODO: 
    // if(initOpenGLState() != 0) 
    //     throw opengl_initialization_failure("Terrain::initialize() > InitOpenGLState() failed\n");
	if(result == 0) 
		logFile.log("GuassianBlur::initialize() > opengl state initialized\n"); 

    assert(fbos[0].createFBOWithoutDepthBuffer(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	assert(fbos[1].createFBOWithoutDepthBuffer(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
	logFile.log("GuassianBlur::initialize() > fbos initialized\n"); 

    assert(quad.initialize() == 0); 

	isInitialized = true; 
    logFile.log("------ GuassianBlur::initialize() completed ----\n"); 
    return 0; 
} 

int 
GuassianBlur::initOpenGLState() 
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
	
    shaderProgram.create(shaders, attributes); 

    // get uniform locations 
    textureSamplerUniform = shaderProgram.getUniformLocation("uTextureSampler"); 
    horizontalOrVerticalUniform = shaderProgram.getUniformLocation("uHorizontalOrVertical"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

GLuint 
GuassianBlur::render(GLuint brightColorTexture, int blurIterations) 
{
	// TODO: 
	// if(isInitialized == false) 
	// 	throw called_render_before_initialization("GuassianBlur::render() > initialized first\n"); 

    bool fboPingPong = false; 
	for(unsigned int i = 0; i < blurIterations; ++i) 
	{
		FBO fboToBeBound; 
		GLuint inputTexture; 
		bool horizontalOrVertical = 0; 
		
		if(fboPingPong == false) 
		{
			fboToBeBound = fbos[0]; 
			inputTexture = fbos[1].getColorTextureID(); 
			horizontalOrVertical = 0; 
		} 
		else 
		{
			fboToBeBound = fbos[1]; 
			inputTexture = fbos[0].getColorTextureID(); 
			horizontalOrVertical = 1; 
		} 

		if(i==0) 
			inputTexture = brightColorTexture; 
		
		shaderProgram.use(); 
		fboToBeBound.bind(); 

		glActiveTexture(GL_TEXTURE0); 
		glBindTexture(GL_TEXTURE_2D, inputTexture); 
		glUniform1i(textureSamplerUniform, 0); 
		glUniform1i(horizontalOrVerticalUniform, horizontalOrVertical); 
		quad.render(); 
		glUseProgram(0); 

		glBindFramebuffer(GL_FRAMEBUFFER, 0); 

		fboPingPong = !fboPingPong; 
	} 

    return fbos[0].getColorTextureID(); 
} 

void 
GuassianBlur::uninitialize() 
{
	logFile.log("------ GuassianBlur::uninitialize() started ----\n"); 
	if(fbos[0].getColorTextureID() != 0) 
		fbos[0].destroyFBO(); 
	if(fbos[1].getColorTextureID() != 0) 
		fbos[1].destroyFBO(); 
	logFile.log("GuassianBlur::uninitialize() > fbos uninitialized\n"); 

	logFile.log("------ GuassianBlur::uninitialize() completed ----\n"); 
} 

GuassianBlur::~GuassianBlur() 
{
	isInitialized = false; 
} 
