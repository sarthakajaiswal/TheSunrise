#ifndef _HDR_HPP  
#define _HDR_HPP  

#include "../fileHandler.hpp" 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../objects.hpp" 
#include "../shaderProgram.hpp" 

#include <iostream> 
#include <vector> 

extern FileHandler logFile; 

class Exposure 
{
    private: 
        ShaderProgram shaderProgramObject; 
        GLuint mvpMatrixUniform; 
        GLuint hdrSceneTextureUniform; 
        GLuint exposureUniform;  
        
        Quad quad; 

        GLuint sceneTexture; 

        int initOpenGLState(); 

    public: 
        Exposure(); 
        int initialize(); 
        void render(GLuint sceneHDRTexture, float exposure);  
        void uninitialize(); 
}; 

#endif // _HDR_HPP  
