#pragma once  

#include "../global_header.hpp" 
#include "../objects.hpp" 
#include "../shaderProgram.hpp" 
#include "../camera.hpp" 
#include "../fbo.hpp" 
#include "../alphabets.hpp" 
#include "../effects/guassianBlur.hpp" 
#include "../effects/textureBlending.hpp" 
#include "../imgui_manager.hpp" 
#include "../spline.hpp" 

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
        GLuint blendStrengthUniform_heading = 0; 
        
        ShaderProgram brightColorSeparatorProgram; 
        GLuint textureUniform_brightColors = 0; 

        FullScreenTexturer fsTexture; 

        // ===== EFFECTS ===== 
        GuassianBlur blurEffect; 
        TextureBlending blendTextureEffect; 

        // ====== FBOs ======= 
        FBO fbo_scene; 
        FBO fbo_brightColors; 

        // ==== TEXTURES ====== 
        GLuint texture_marbleColor = 0; 
        GLuint texture_marbleNormalMap = 0; 
        GLuint blurTexture; 
        GLuint blendedTexture; 
        GLuint crackTexture; 

        // ===== event handeling related ====== 
        bool bShowCrackTexure = false;
        enum Shot shotNumber = SHOT1; 

        // parameter tweaking 
        float cameraT = 0.0; 

    public: 
        float alphabetSpacing = 3.799; 
        int blurIterations = 2; 
        float blendStrength = 0.0; // for blending red color on marble cracks 
        float astrmomedicompStartingX = -30.540f; 
        float astromedicompZ = 0.0; 
        float alphabetSx = 1.471f, alphabetSy = 2.706f, alphabetSz = 2.647f; 
        float textureAlpha = 1.0f; // for fade effect 

        // lighting related variable 
        GLfloat lightAmbient[4] = {0.1f, 0.1f, 0.1f, 1.0}; 
        GLfloat lightDiffuse[4] = {2.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat lightSpecular[4] = {2.0f, 0.0f, 0.0f, 1.0f};
        // GLfloat lightPosition[4] = {13.57f, 18.041f, -3.767f, 1.0f}; 
        GLfloat lightPosition[4] = {0.0f, 50.0f, 50.0f, 1.0f}; 

        GLfloat materialAmbient[4] = {0.0f, 0.0f, 0.0f, 1.0f}; 
        GLfloat materialDiffuse[4] = {1.0f, 2.0f, 0.0f, 1.0f}; 
        GLfloat materialSpecular[4] = {0.7f, 0.7f, 0.7f, 1.0f}; 
        GLfloat materialShininess = 128.0f; 

        IntroScene(); 

        int initialize(); 
        void display(); 
        void update(); 
        void uninitialize(); 
        
        ~IntroScene(); 

        bool initHeadingAlphabetsShaderProgram(); 
        bool initBrightColorSeparatorProgram(); 

        void eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
}; 

