#ifndef _COMMON_HPP 
#define _COMMON_HPP 

// standard headers 
#include <stdexcept> 

// custom headers 
#include "global_header.hpp" 
#include "stb_image.h" 

// global function declarations 
float degToRad(float degree); 

// ============================================= TEXTURE =======================================

class unable_to_load_file : public std::runtime_error 
{
    public: 
        unable_to_load_file(const char* msg) : std::runtime_error(msg) 
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

        static Texture loadTexture(const char* fileName, BOOL bFlipImage); 
}; 

// ============================================= VERTEX =======================================

#endif /* _COMMON_HPP */ 

