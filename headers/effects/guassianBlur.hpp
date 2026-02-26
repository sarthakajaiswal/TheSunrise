#pragma once 

#include <iostream>
#include <vector> 
#include <cmath> 
#include <stdexcept> 
#include <cassert> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "..\fbo.hpp" 
#include "..\shaderProgram.hpp" 
#include "..\objects.hpp"

class GuassianBlur 
{
    private: 
        ShaderProgram shaderProgram; 
        GLuint textureSamplerUniform; 
        GLuint horizontalOrVerticalUniform; 

        FBO fbos[2]; 

        Quad quad; 

        bool isInitialized; 

        int initOpenGLState(); 

    public: 
        GuassianBlur(); 
        int initialize(); 
        GLuint render(GLuint brightColorTexture, int blurIterations); 
        void uninitialize(); 
        ~GuassianBlur(); 
}; 

