#pragma once 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 

class Fog 
{
    private: 
        ShaderProgram fogProgram; 
        GLuint mvpMatrixUniform = 0; 
        GLuint modelViewMatrixUniform = 0; 
        GLuint cameraPositionUniform = 0; 
        GLuint fogStartUniform = 0; 
        GLuint fogEndUniform = 0; 
        GLuint fogColorUniform = 0; 

        int initShaderProgram(); 

    public: 
        Fog(); 
        int initialize(); 
        void useProgram(mat4 _mvpMatrix, mat4 _mvMatrix, vec3 _camPosition, float _fogStart, float _fogEnd, vec3 _fogColor); 
        void unuseProgram(); 
        void uninitialize(); 
}; 

