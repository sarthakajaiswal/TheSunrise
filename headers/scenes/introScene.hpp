#pragma once  

#include "../global_header.hpp" 
#include "../objects.hpp" 
#include "../shaderProgram.hpp" 
#include "../camera.hpp" 
#include "../fbo.hpp" 
#include "../texture.hpp"
#include "../alphabets.hpp" 

class IntroScene 
{
    private: 
        Quad quad; 

        // ====== SHADER PROGRAM AND UNIFORMS ====== 
        ShaderProgram headingAlphabetsShaderProgram; 
        GLuint modelViewMatrixUniform_heading = 0; 
        GLuint projectionMatrixUniform_heading = 0; 
        GLuint lightPositionUniform_heading = 0; 
        GLuint laUniform_heading = 0; 
        GLuint ldUniform_heading = 0; 
        GLuint lsUniform_heading = 0; 
        GLuint materialShininessUniform_heading = 0; 
        GLuint kaUniform_heading = 0; 
        GLuint kdUniform_heading = 0; 
        GLuint ksUniform_heading = 0; 
        GLuint normalMapTextureSamplerUniform_heading = 0; 
        GLuint colorTextureSamplerUniform_heading = 0; 
        GLuint normalMapEnableUniform_heading = 0; 
        GLuint blendStrengthUniform_heading = 0; 
        
        ShaderProgram brightColorSeparatorProgram; 
        GLuint textureUniform_brightColors = 0; 

        ShaderProgram guassianBlurProgram; 
        GLuint textureSamplerUniform_guassianBlur = 0; 
        GLuint horizontalOrVerticalUniform_guassianBlur = 0; 

        ShaderProgram blendTextureProgram; 
        GLuint texture1Uniform_blendProgram = 0; 
        GLuint texture2Uniform_blendProgram = 0; 

        ShaderProgram fsTextureProgram; 
        GLuint textureUniform_fsTexture = 0; 

        // ====== FBOs ======= 
        FBO fbo_scene; 
        FBO fbo_brightColors; 
        FBO fbos_guassianBlur[2]; 

        // ==== TEXTURES ====== 
        GLuint texture_marbleColor = 0; 
        GLuint texture_marbleNormalMap = 0; 

    public: 
        int blurIterations = 10; 
        float blendStrength = 1.0; // for blending red color on marble cracks 

        // lighting related variable 
        GLfloat lightAmbient[4] = {0.1f, 0.1f, 0.1f, 1.0}; 
        GLfloat lightDiffuse[4] = {2.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat lightSpecular[4] = {2.0f, 0.0f, 0.0f, 1.0f};
        GLfloat lightPosition[4] = {100.0f, 100.0f, 100.0f, 1.0f}; 

        GLfloat materialAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat materialDiffuse[4] = {1.0f, 2.0f, 0.0f, 1.0f}; 
        GLfloat materialSpecular[4] = {0.7f, 0.7f, 0.7f, 1.0f}; 
        GLfloat materialShininess = 128.0f; 

        // feature toggle 
        bool bNormalMapping = false; 

        IntroScene(); 

        int initialize(); 
        void display(); 
        void update(); 
        void uninitialize(); 

        bool initHeadingAlphabetsShaderProgram(); 
        bool initBrightColorSeparatorProgram(); 
        bool initGuassianBlurProgram(); 
        bool initTextureBlendProgram(); 
        bool initializeFullScreenTextureProgram(); 

        ~IntroScene(); 
}; 

