#include "..\..\headers\effects\textureBlending.hpp" 

TextureBlending::TextureBlending() 
{
} 

int TextureBlending::initialize() 
{
    logFile.log("---- TextureBlending::initialize() started --\n"); 

    int result = initOpenGLState(); 
    // TODO: 
    // if(initOpenGLState() != 0) 
    //     throw opengl_initialization_failure("Terrain::initialize() > InitOpenGLState() failed\n");
    if(result == 0) 
		logFile.log("TextureBlending::initialize() > opengl state initialized\n"); 

    assert(fbo.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true); 
    logFile.log("TextureBlending::initialize() > fbo created succesfully\n"); 
    
    assert(quad.initialize() == 0); 
    logFile.log("TextureBlending::initialize() > quad initialized\n"); 

    logFile.log("---- TextureBlending::initialize() completed --\n"); 
    return 0; 
} 

int TextureBlending::initOpenGLState()
{
    // variable declarations 
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
 
    // code 
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
	
    shaderProgram.create(shaders, attributes); 

    // get uniform locations 
    texture1Uniform = shaderProgram.getUniformLocation("uTexture1"); 
    texture2Uniform = shaderProgram.getUniformLocation("uTexture2");  
    texture1FactorUniform = shaderProgram.getUniformLocation("uTex1Factor"); 
    texture2FactorUniform = shaderProgram.getUniformLocation("uTex2Factor"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    return (0); 
} 

GLuint TextureBlending::render(GLuint texture1, float tex1Factor, GLuint texture2, float tex2Factor) 
{
    // function declarations 
    void resize(int width, int height); 

    // code 
    fbo.bind(); 

    resize(winWidth, winHeight); 
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

    shaderProgram.use();  

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture1); 
	glUniform1i(texture1Uniform, 0);
    glUniform1f(texture1FactorUniform, tex1Factor); 

	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, texture2); 
	glUniform1i(texture2Uniform, 1); 
    glUniform1f(texture2FactorUniform, tex1Factor); 

	quad.render(); 

	glBindTexture(GL_TEXTURE_2D, 0); 
	glUseProgram(0); 

    fbo.unbind(); 
    
    return (fbo.getTextureID()); 
} 

void TextureBlending::uninitialize() 
{
    // code 
    logFile.log("TextureBlending::uninitialize() uninitializing textureBlending...\n"); 

    if(fbo.getTextureID() != 0) 
        fbo.destroyFBO(); 
} 

TextureBlending::~TextureBlending() 
{
} 
