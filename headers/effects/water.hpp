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
                int initialize(float textureScale=1.0); 
                void render(); 
                void uninitialize(); 
        }; 

        ShaderProgram shaderProgram; 
        GLuint modelMatrixUniform; 
        GLuint viewMatrixUniform; 
        GLuint projectionMatrixUniform; 
        GLuint tilingUniform; 
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

        WaterRect waterRect; 

        GLuint dudvTexture; 
        GLuint normalMap; 
        float textureScale; 

        float deltaTime; 
        LARGE_INTEGER startTime, currentTime, freq;
        float one_divided_by_freq; 

        int initOpenGLState(); 

    public: 
        FBO reflectionFBO; 
        FBO refractionFBO; 

        Water(); 
        int initialize(float textureScale); 
        void render(mat4 modelMatrix, mat4 viewMatrix, mat4 projectionMatrix, vec3 cameraPosition, vec3 lightPosition, float waveSpeed); 
        void update(); 
        void uninitialize(); 
        ~Water(); 
}; 

