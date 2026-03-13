#include "..\..\headers\effects\fog.hpp" 

Fog::Fog()
{
} 

int Fog::initialize() 
{
    assert(initShaderProgram() == 0); 
} 

void Fog::useProgram(mat4 _mvpMatrix, mat4 _mvMatrix, vec3 _camPosition, float _fogStart, float _fogEnd, vec3 _fogColor) 
{
    glUseProgram(fogProgram.getID()); 

	glUniform3fv(cameraPositionUniform, 1, _camPosition); 
	glUniform1f(fogStartUniform, _fogStart); 
	glUniform1f(fogEndUniform, _fogEnd); 
	glUniform3fv(fogColorUniform, 1, _fogColor);  

    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, _mvpMatrix);  
    glUniformMatrix4fv(modelViewMatrixUniform, 1, GL_FALSE, _mvMatrix);  
} 

void Fog::unuseProgram() 
{
    glUseProgram(0); 
} 

int Fog::initShaderProgram() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/fog.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/fog.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    
    fogProgram.create(shaders, attributes); 

    // get uniform locations 
	mvpMatrixUniform = fogProgram.getUniformLocation("uMVPMatrix"); 
	modelViewMatrixUniform = fogProgram.getUniformLocation("uModelViewMatrix"); 
	cameraPositionUniform = fogProgram.getUniformLocation("uCameraPosition"); 
	fogStartUniform = fogProgram.getUniformLocation("uFogStart"); 
	fogEndUniform = fogProgram.getUniformLocation("uFogEnd"); 
	fogColorUniform = fogProgram.getUniformLocation("uFogColor"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 
