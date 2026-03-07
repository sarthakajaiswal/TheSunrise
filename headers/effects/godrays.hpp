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
    private: 
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

        // pass4 
        ShaderProgram finalCompositeProgram; 
        GLuint sceneTextureUniform_finalComposite = 0; 
        GLuint godRaysTextureUniform_finalComposite = 0; 
        GLuint godRaysStrengthUniform_finalComposite = 0; 

        FBO occlusionFBO; 
        FBO sceneFBO; 
        FBO motionBlurFBO; 
        FBO finalCompositeFBO; 

        FullScreenTexturer fsTexturer; 
        Quad quad; 

        int initOpenGLState(); 
        int initOcclusionProgram(); 
        int initSceneProgram(); 
        int initMotionBlurProgram(); 
        int initFinalCompositeProgram(); 

    public: 
        GLuint createOcclusionTexture(); 
        GLuint createSceneTexture(); 
        GLuint createMotionBlurTexture(float exposure, float decay, float density, float weight, int numSamples, vmath::vec2 lightPositionOnScreen); 
        GLuint getFinalCompositeTexture(); 
        
        FBO sceneObjectsFBO; 
        FBO lightSourceFBO; 

        Godrays(); 
        int initialize(); 
        GLuint render(vmath::mat4 viewMatrix, vmath::mat4 projectionMatrix, float exposure, float decay, float density, float weight, int numSamples, vmath::vec3 lightPosition); 
        void uninitialize(); 
}; 

vec2 convertToScreenSpace(vec3 worldPos, mat4 viewMatrix, mat4 projectionMatrix); 

#endif // _GODRAYS_HPP   
