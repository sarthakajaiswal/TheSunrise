#include "..\..\headers\effects\exposure.hpp" 

Exposure::Exposure()
{
} 

int 
Exposure::initialize() 
{
    logFile.log("---- Exposure::initialize() started ----\n");
    assert(initOpenGLState() == 0); 
    assert(quad.initialize() == 0); 
    logFile.log("---- Exposure::initialize() completed successfully ----\n");
    return (0); 
} 

void 
Exposure::render(GLuint hdrSceneTexture, float exposure) 
{
    shaderProgramObject.use(); 

    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, hdrSceneTexture); 
    glUniform1i(hdrSceneTextureUniform, 0); 
    glUniform1f(exposureUniform, exposure); 

    quad.render(); 

    shaderProgramObject.unuse(); 
} 

int 
Exposure::initOpenGLState() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/exposure.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/exposure.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    shaderProgramObject.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform = shaderProgramObject.getUniformLocation("uMVPMatrix"); 
    exposureUniform = shaderProgramObject.getUniformLocation("uExposure"); 
    hdrSceneTextureUniform = shaderProgramObject.getUniformLocation("uHDRTextureSampler"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

void 
Exposure::uninitialize() 
{
    logFile.log("---- Exposure::uninitialize() started ----\n");
    logFile.log("---- Exposure::uninitialize() completed successfully ----\n");
} 
