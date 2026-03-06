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
        
        ShaderProgram sceneFromObjectsAndLightProgram; 
        GLuint objectsTextureUniform_scene; 
        GLuint lightSourceTextureUniform_scene;  
        
        FBO sceneObjectsFBO; 
        FBO lightSourceFBO; 
        FBO occlusionFBO; 
        FBO sceneFBO; 

        Quad quad; 

        int initOpenGLState(); 
        int initOcclusionProgram(); 
        int initSceneProgram(); 

        GLuint getOcclusionTexture(); 
        GLuint getSceneTexture(); 

    public: 
        Godrays(); 
        int initialize(); 
        void uninitialize(); 
}; 

#endif // _GODRAYS_HPP   
