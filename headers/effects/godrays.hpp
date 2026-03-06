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
        ShaderProgram silhoutteFromObjectsAndLightProgram; 
        GLuint objectsTextureUniform_silhotte; 
        GLuint lightSourceTextureUniform_silhoutte;  
        
        FBO sceneObjectsFBO; 
        FBO lightSourceFBO; 
        FBO occlusionFBO; 

        Quad quad; 

        int initOpenGLState(); 
        GLuint getOcclusionTexture(); 
        GLuint getSceneTexture(); 

    public: 
        Godrays(); 
        int initialize(); 
        void uninitialize(); 
}; 

#endif // _GODRAYS_HPP   
