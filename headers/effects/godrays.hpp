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

/* 
    Input 
    Occlusion texture via occlusionFBO 
    scene texture via parameter  

    Flow: 
    Occclusion FBO is created by user itself in occlusionFBO 
    scene texture is also given by user as parameter to renderWithScene() 
    Godrays class applied radial blur to create godrays texture in godraysFBO 
    and then in renderWithScene() godrays texture and scene texture are combined to get final output on screen 
*/ 

class Godrays 
{
    private: 
        ShaderProgram radialBlurProgram; 
        GLuint exposureUniform_radialBlur = 0; 
        GLuint decayUniform_radialBlur = 0; 
        GLuint densityUniform_radialBlur = 0; 
        GLuint weightUniform_radialBlur = 0; 
        GLuint lightPositionOnScreenUniform_radialBlur = 0; 
        GLuint occlusionTextureUniform_radialBlur = 0; 
        GLuint numSamplesUniform_radialBlur = 0; 

        ShaderProgram finalCompositeProgram; 
        GLuint sceneTextureUniform_finalComposite = 0; 
        GLuint godRaysTextureUniform_finalComposite = 0; 
        GLuint godRaysStrengthUniform_finalComposite = 0; 

        FBO godraysFBO; 
        Quad quad; 

        int initRadialBlurProgram(); 
        int initFinalCompositeProgram(); 

        GLuint createRadialBlurTexture(GLuint occlusionTexture, float exposure, float decay, float density, float weight, int numSamples, vmath::vec2 lightPositionOnScreen); 

    public:         
        FBO occlusionFBO; 

        Godrays(); 
        int initialize(); 
        void renderWithScene(GLuint sceneTexture, vmath::mat4 viewMatrix, vmath::mat4 projectionMatrix, float exposure, float decay, float density, float weight, float strength, int numSamples, vmath::vec3 lightPosition); 
        void uninitialize(); 
}; 

vec2 convertToScreenSpace(vec3 worldPos, mat4 viewMatrix, mat4 projectionMatrix); 

#endif // _GODRAYS_HPP   
