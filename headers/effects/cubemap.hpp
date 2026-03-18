#ifndef _CUBEMAP_HPP 
#define _CUBEMAP_HPP 

#include "../fileHandler.hpp" 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 

#include <iostream> 
#include <vector> 

extern FileHandler logFile; 

class Cubemap 
{
    private: 
        ShaderProgram shaderProgramObject; 
        GLuint rotationMatrixUniform = 0; 
        GLuint viewMatrixUniform = 0; 
        GLuint projectionMatrixUniform = 0; 
        GLuint skyboxSamplerUniform = 0;  
        
        Cube cube; 

        GLuint cubemapTexture; 
        void loadCubeMapTexture(const char* images[]);

        int initOpenGLState(); 


    public: 
        Cubemap(); 
        int initialize(const char* cubemapImages[6]); 
        void render(vmath::mat4 _rotationMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix); 
        void uninitialize(); 
}; 

#endif // _CUBEMAP_HPP 
