#include "..\..\headers\effects\cubemap.hpp" 

Cubemap::Cubemap()
{
} 

int Cubemap::initialize(const char* cubemapImages[6]) 
{
    logFile.log("---- Cubemap::initialize() started ----\n");

    assert(initOpenGLState() == 0); 
    assert(cube.initialize() == 0); 

    loadCubeMapTexture(cubemapImages); 
    if(cubemapTexture == 0) 
        throw texture_loading_failure("Cubemap::initialize() > failed to load cubemap texture\n"); 

    logFile.log("---- Cubemap::initialize() completed successfully ----\n");
    return (0); 
} 

void Cubemap::render(vmath::mat4 _rotationMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix) 
{
    shaderProgramObject.use(); 

    glUniformMatrix4fv(rotationMatrixUniform, 1, GL_FALSE, _rotationMatrix);  
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, _viewMatrix);  
	glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, _projectionMatrix);  

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture); 
	glUniform1i(skyboxSamplerUniform, 0); 

    cube.render(); 

    shaderProgramObject.unuse(); 
} 

int Cubemap::initOpenGLState() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/cubemap.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/cubemap.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    
    shaderProgramObject.create(shaders, attributes); 

    // get uniform locations 
	rotationMatrixUniform = shaderProgramObject.getUniformLocation("uRotationMatrix"); 
	viewMatrixUniform = shaderProgramObject.getUniformLocation("uViewMatrix"); 
	projectionMatrixUniform = shaderProgramObject.getUniformLocation("uProjectionMatrix"); 
	skyboxSamplerUniform = shaderProgramObject.getUniformLocation("uSkyboxSampler"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

void Cubemap::loadCubeMapTexture(const char* images[])
{
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    stbi_set_flip_vertically_on_load(false);

    for (unsigned int i = 0; i < 6; ++i)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(images[i], &width, &height, &nrChannels, 4);

        if (data)
        {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                width,
                height,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
            logFile.log("%dth image loaded\n", i);
        }
        else
        {
            logFile.log("Failed to load image: %s\n", images[i]);
        }
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

