#pragma once 

#include "global_header.hpp" 
#include "common.hpp" 

class Butterfly 
{
    private: 
        GLuint shaderProgram; 
        GLuint mvpMatrixUniform; 

        Quad quad; 
        
        GLuint texture; 
        float wingAngle; 

        void initShaderProgram(void); 

    public: 
        Butterfly(); 
        int initialize(const char* wingTextureImagePath); 
        void render(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix); 
        void update(float wingSpeed); 
        ~Butterfly(); 
}; 
