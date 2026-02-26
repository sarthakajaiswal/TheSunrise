#include "../headers/global_header.hpp" 
#include "../headers/shaderProgram.hpp" 

extern FileHandler logFile; 
void uninitialize(void); 

ShaderProgram::ShaderProgram() 
{
    this->id = 0; 
}  

GLuint ShaderProgram::getID() 
{
    return this->id; 
} 

int ShaderProgram::create(std::vector<ShaderSourceCodeAndType> shaderSourceAndTypes, std::vector<AttributeWithIndexLocation> attributes) 
{
    // code 
    // create attack and link shader program objects 
    // (1) create shader program object 
    id = glCreateProgram();
    
    // create an array of shader objects to be attached with program 
    std::vector<ShaderObject> shaderObjects; 
    for(int i = 0; i < shaderSourceAndTypes.size(); ++i) 
    {
        shaderObjects.push_back(
            ShaderObject(shaderSourceAndTypes[i].sourceCode, shaderSourceAndTypes[i].type)
        );   
    } 

    // (2) attach shader objects to shader program objects 
    for(int i = 0; i < shaderObjects.size(); ++i) 
        glAttachShader(id, shaderObjects[i].id); 

    // bind attribute locations before linking 
    for(int i = 0; i < attributes.size(); ++i) 
        glBindAttribLocation(id, attributes[i].index, attributes[i].attributeName); 

    // (3) tell to link shader objects to shader program object 
    glLinkProgram(id); 

    // (4) check for link error log 
    GLint status = 0; 
    GLint infoLogLength = 0; 
    GLchar* szInfoLog = NULL; 

    glGetProgramiv(id, GL_LINK_STATUS, &status); 
    if(status == GL_FALSE) 
    {
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength); 
        if(infoLogLength > 0) 
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar)); 
            if(szInfoLog != NULL) 
            {
                glGetProgramInfoLog(id, infoLogLength, NULL, szInfoLog); 
                logFile.log("Shader program link log = %s\n", szInfoLog); 
                free(szInfoLog); 
                szInfoLog = NULL; 
            } 
        } 

        uninitialize(); 
    }
    
    logFile.log("ShaderProgram::ShaderProgram() > program created successfully\n"); 
    return (0); 
} 

void ShaderProgram::use() 
{
    // code 
    glUseProgram(this->id); 
} 

void ShaderProgram::unuse() 
{
    // code 
    glUseProgram(0); 
} 

GLuint ShaderProgram::getUniformLocation(const char* uniformName) 
{
    // code 
    return (glGetUniformLocation(this->id, uniformName)); 
} 

ShaderProgram::~ShaderProgram() 
{
    // detach, delete shader objects and delete shaderprogram 
    logFile.log("ShaderProgram::~ShaderProgram() > uninitializing...\n"); 
    // (1) check of shader program object is still here 
    if(this->id) 
    {
        GLint numShaders = 0; 

        glUseProgram(this->id); 
        {
            // () get number of shader objects attached, continue only if number is >0
            glGetProgramiv(this->id, GL_ATTACHED_SHADERS, &numShaders); 
            if(numShaders > 0) 
            {
                // (3) create a buffer/array to hold obtained number and continue only if malloc succeeds 
                GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint)); 
                if(NULL == pShaders) 
                {
                    // (4) get sgader objects into this buffer 
                    glGetAttachedShaders(this->id, numShaders, NULL, pShaders); 

                    // (5) loop for obtained number of shaders, Use this loop to detach and delete every shader object from array 
                    for(GLint i = 0; i < numShaders; ++i) 
                    {
                        glDetachShader(this->id, pShaders[i]); 

                        // TODO: delete shader object (see TODO from ShaderObject::~ShaderObject()) 
                        // glDeleteShader(pShaders[i]); 
                        // pShaders[i] = 0; 
                    } 
                } 

                // (6) free buffer 
                free(pShaders); 
                pShaders = NULL; 
            } 
        }  
        glUseProgram(0); 

        // (7) delete program shader object 
        glDeleteProgram(this->id); 
        this->id = 0; 
    }
} 

// ============== HELPER CLASSES =================== 
// (1) 
ShaderSourceCodeAndType::ShaderSourceCodeAndType(const char* _sourceCode, GLenum _type) 
{
    this->type = _type; 
    this->sourceCode = _sourceCode; 
} 

AttributeWithIndexLocation::AttributeWithIndexLocation(int _index, const char* _attributeName) 
{
    this->index = _index; 
    this->attributeName = _attributeName; 
} 

ShaderProgram::ShaderObject::ShaderObject(const char* _sourceCode, GLenum _type) 
{
    this->type = _type; 
    this->sourceCode = _sourceCode; 

    this->id = glCreateShader(this->type); 
    glShaderSource(this->id, 1, (const char**)&(this->sourceCode), NULL); 
    glCompileShader(this->id); 

    GLint status = 0; 
    GLint infoLogLength = 0; 
    GLchar* szInfoLog = NULL; 

    glGetShaderiv(this->id, GL_COMPILE_STATUS, &status); 
    if(status == GL_FALSE) 
    {
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &infoLogLength); 
        if(infoLogLength > 0) 
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(char)); 
            if(szInfoLog != NULL) 
            {
                glGetShaderInfoLog(this->id, infoLogLength, NULL, szInfoLog); 
                logFile.log("shader compilation log = %s\n", szInfoLog); 
                free(szInfoLog); 
                szInfoLog = NULL; 
            }
        }  
        else 
        {
            logFile.log("shader compiled succesgully\n"); 
        } 

        uninitialize(); 
    } 
} 

ShaderProgram::ShaderObject::~ShaderObject() 
{
    // TODO: we are not deleting shader object here, because shaderObject get deleted in ShaderProgram::create() while pushing into shaderObjects array 
    // glDeleteShader(this->id); 
    // this->id = 0; 
} 