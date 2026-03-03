#ifndef _COMMON_HPP 
#define _COMMON_HPP 

// standard headers 
#include <stdexcept> 

// custom headers 
#include "global_header.hpp" 
#include "stb_image.h" 
#include "shaderProgram.hpp" 
#include "objects.hpp" 

// global function declarations 
float degToRad(float degree); 

struct Vertex 
{
	vmath::vec3 position; 
	vmath::vec3 normal; 
	vmath::vec2 texCoord; 
	vmath::vec3 tangent; 
	vmath::vec3 bitangent; 
}; 

// ========= EXCEPTIONS ===================
class called_render_before_initialization : public std::runtime_error 
{
    public: 
        called_render_before_initialization(const char* msg) : std::runtime_error(msg) 
        {
        } 
}; 
// ========= EXCEPTIONS ===================

// ======== TEXTURE ===========

class unable_to_load_file : public std::runtime_error 
{
    public: 
        unable_to_load_file(const char* msg) : std::runtime_error(msg) 
        {} 
}; 

class texture_loading_failure : public std::runtime_error 
{
    public: 
        texture_loading_failure(const char* msg) : std::runtime_error(msg) 
        {} 
}; 

class Texture 
{
    private: 
        unsigned int id; 
        int width; 
        int height; 
        int components; 
    
    public: 
        Texture(); 
        ~Texture(); 

        // getters 
        unsigned int getID() const; 
        int getWidth() const; 
        int getHeight() const; 
        int getComponents() const; 

        static Texture loadTexture(const char* fileName, BOOL bFlipImage=TRUE); 
}; 

GLuint loadTexture(const char* pathToImage, BOOL bFlipImage=TRUE); 

// ========= TEXTURE ===========

// ========= FS TEXTURE PROGRAM ======== 
class FullScreenTexturer 
{
    private: 
        ShaderProgram shaderProgram; 
        GLuint textureUniform; 

        Quad quad; 

    public: 
        int initialize();
        void render(GLuint texture); 
}; 
// ========= FS TEXTURE PROGRAM ======== 

#endif /* _COMMON_HPP */ 

