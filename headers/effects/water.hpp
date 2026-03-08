#pragma once 

#include <vector> 
#include <cassert> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "..\fbo.hpp" 
#include "..\shaderProgram.hpp" 
#include "..\common.hpp" 

class Water 
{
    private: 
        class WaterRect 
        {
            private: 
                GLuint vao; 
                GLuint vbo; 

            public: 
                WaterRect(); 
                int initialize(); 
                void render(); 
                void uninitialize(); 
        }; 

        ShaderProgram shaderProgram; 
        GLuint modelMatrixUniform; 
        GLuint viewMatrixUniform; 
        GLuint projectionMatrixUniform; 
        GLuint reflectionTextureUniform; 
        GLuint refractionTextureUniform; 
        GLuint dudvTextureUniform; 
        GLuint normalMapUniform; 
        GLuint depthMapUniform; 
        GLuint screenSizeUniform; 
        GLuint moveFactorUniform; 
        GLuint cameraPositionUniform; 
        GLuint lightPositionUniform; 
        GLuint lightColorUniform; 

        ShaderProgram bwShader; 
        GLuint mvpMatrixUniform_bwShader; 

        WaterRect waterRect; 

        GLuint dudvTexture; 
        GLuint normalMap; 

        float deltaTime; 
        LARGE_INTEGER startTime, currentTime, freq;
        float one_divided_by_freq; 

        bool isInitialized; 

        int initOpenGLState(); 
        int initBWShader(); 

    public: 
                FBO reflectionFBO; 
        FBO refractionFBO; 

        Water(); 
        int initialize(); 
        void render(mat4 modelMatrix, mat4 viewMatrix, mat4 projectionMatrix, vec3 cameraPosition, vec3 lightPosition, float waveSpeed); 
        void update(); 
        void uninitialize(); 
        ~Water(); 
}; 

