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
        // pass1 
        ShaderProgram silhoutteFromObjectsAndLightProgram; 
        GLuint objectsTextureUniform_silhotte = 0; 
        GLuint lightSourceTextureUniform_silhoutte = 0;  
        
        // pass2 
        ShaderProgram sceneFromObjectsAndLightProgram;  
        GLuint objectsTextureUniform_scene = 0; 
        GLuint lightSourceTextureUniform_scene = 0;  
        
        // pass3 
        ShaderProgram motionBlurProgram; 
        GLuint exposureUniform_motionBlur = 0; 
        GLuint decayUniform_motionBlur = 0; 
        GLuint densityUniform_motionBlur = 0; 
        GLuint weightUniform_motionBlur = 0; 
        GLuint lightPositionOnScreenUniform_motionBlur = 0; 
        GLuint occlusionTextureUniform_motionBlur = 0; 
        GLuint numSamplesUniform_motionBlur = 0; 

        FBO sceneObjectsFBO; 
        FBO lightSourceFBO; 
        FBO occlusionFBO; 
        FBO sceneFBO; 
        FBO motionBlurFBO; 

        Quad quad; 

        int initOpenGLState(); 
        int initOcclusionProgram(); 
        int initSceneProgram(); 
        int initMotionBlurProgram(); 

        GLuint getOcclusionTexture(); 
        GLuint getSceneTexture(); 
        GLuint getMotionBlurTexture(); 

    public: 
        Godrays(); 
        int initialize(); 
        void uninitialize(); 
}; 

#endif // _GODRAYS_HPP   
