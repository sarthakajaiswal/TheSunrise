#pragma once 

#include <vector> 
#include <cassert> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "..\fbo.hpp" 
#include "..\shaderProgram.hpp" 
#include "..\objects.hpp"

class TextureBlending 
{
    private: 
        ShaderProgram shaderProgram; 
        GLuint texture1Uniform; 
        GLuint texture2Uniform; 

        FBO fbo; 

        Quad quad; 

        bool isInitialized; 

        int initOpenGLState(); 

    public: 
        TextureBlending(); 
        int initialize(); 
        GLuint render(GLuint texture1, GLuint texture2); 
        void uninitialize(); 
        ~TextureBlending(); 
}; 

