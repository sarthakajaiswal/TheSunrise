#ifndef _GODRAYS_HPP   
#define _GODRAYS_HPP   

#include "../fileHandler.hpp" 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../objects.hpp"
#include "../fbo.hpp"  
#include "../shaderProgram.hpp" 

#include <iostream> 
#include <vector> 

extern FileHandler logFile; 

class Godrays 
{
    public: 
        ShaderProgram occlusionProgram; 
        GLuint mvpMatrixUniform_occlusion; 
        GLuint isLightUniform_occlusion; 
        
        FBO sceneObjectsFBO; 
        FBO lightSourceFBO; 

        int initOpenGLState(); 

    public: 
        Godrays(); 
        int initialize(); 
        void uninitialize(); 
}; 

#endif // _GODRAYS_HPP   
