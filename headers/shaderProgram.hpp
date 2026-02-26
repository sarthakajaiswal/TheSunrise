#ifndef _SHADER_PROGRAM_HPP 
#define _SHADER_PROGRAM_HPP 

#include <iostream> 
#include <vector> 

class ShaderSourceCodeAndType 
{
    public: 
        GLenum type; 
        const char* sourceCode; 

    public: 
        ShaderSourceCodeAndType(const char* sourceCode, GLenum type); 
}; 

class AttributeWithIndexLocation 
{
    public: 
        int index; 
        const char* attributeName; 

    public: 
        AttributeWithIndexLocation(int index, const char* attribute); 
}; 

class ShaderProgram 
{
    friend class ShaderSourceCodeAndType; 
    private: 
        class ShaderObject 
        {
            friend class ShaderSourceCodeAndType; 
            public: 
                const char* sourceCode; 
                GLenum type; 
                GLuint id; 

            public: 
                ShaderObject(const char* sourceCode, GLenum type); 
                ~ShaderObject(); 
        };

        GLuint id; 

    public: 
        GLuint getID(); 

        ShaderProgram(); 
        int create(std::vector<ShaderSourceCodeAndType> shaders, std::vector<AttributeWithIndexLocation> linkingAttributes); 

        void ShaderProgram::use(); 
        static void ShaderProgram::unuse(); 
        GLuint getUniformLocation(const char* uniformName); 
        
        ~ShaderProgram(); 
}; 

#endif /* _SHADER_PROGRAM_HPP */ 
