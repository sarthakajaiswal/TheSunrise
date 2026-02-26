#pragma once 

#include <iostream> 
#include <string> 
#include <vector> 
#include <cmath> 
#include <stdexcept> 
#include <cassert> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "..\vmath.h" 
#include "..\stb_image.h" 
#include "..\fileHandler.hpp" 
#include "..\global_header.hpp" 
#include "..\camera.hpp" 
#include "..\shaderProgram.hpp" 

extern FileHandler logFile; 
extern Camera testSceneCamera; 

struct Vertex 
{
    vmath::vec3 position; 
    vmath::vec2 texCoord; 
    vmath::vec3 normal; 
}; 

class heightmap_loading_failure : public std::runtime_error 
{
    public: 
        heightmap_loading_failure(const char* msg); 
}; 

class opengl_initialization_failure : public std::runtime_error 
{
    public: 
        opengl_initialization_failure(const char* msg); 
}; 

class render_called_before_initialize : public std::runtime_error 
{
    public: 
        render_called_before_initialize(const char* msg); 
}; 

class no_texture_images_provided : public std::runtime_error 
{
    public: 
        no_texture_images_provided(const char* msg); 
}; 

class texture_loading_failure : public std::runtime_error 
{
    public: 
        texture_loading_failure(const char* msg); 
}; 

class Terrain 
{
    private: 
        std::vector<Vertex> mesh; 
        float* heightmapData; 
        int width, depth; 
        float worldScale; 
        float heightScale; 
        float textureScale; 

        std::vector<GLuint> textures; 
        std::vector<float> textureHeightRanges; 

        GLuint shaderProgramObject; 
        GLuint modelMatrixUniform; 
        GLuint viewMatrixUniform; 
        GLuint projectionMatrixUniform; 
        GLuint normalMatrixUniform; 
        GLuint heightScaleUniform; 
        GLuint texture1Uniform; 
        GLuint texture2Uniform; 
        GLuint texture3Uniform; 
        GLuint texture4Uniform; 
        GLuint hRange1Uniform; 
        GLuint hRange2Uniform; 
        GLuint hRange3Uniform; 
        GLuint hRange4Uniform; 
        GLuint viewPositionUniform; 


        std::vector<unsigned int> indices; 
        GLuint vao, vbo, ebo; 

        bool isInitialized; 

        void showHeightMapData(); 
        void showVertices(); 
        void InitVertices(int width, int depth, float* heightData, std::vector<Vertex>& vertices); 
        void InitIndices(int width, int depth, std::vector<unsigned int>& indices); 
        float* LoadHeightMap(const char* filePath, int* pWidth, int* pDepth); 
        int InitOpenGLState(); 
        int loadTexture(const char* path, BOOL bFlipImage = TRUE); 

    public: 
        Terrain(); 
        int initialize(
                const char* heightmapImagePath, float _worldScale, float _heightScale, 
                std::vector<std::string> textureImages, std::vector<float> _textureHeightRanges, float _textureScale
            ); 
        void render(
                vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix, 
                vec3 viewPosition
            ); 
        void uninitialize(); 
        ~Terrain(); 
}; 

