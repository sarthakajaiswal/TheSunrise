#ifndef _OBJECTS_HPP 
#define _OBJECTS_HPP 

#include "global_header.hpp"  
#include "Sphere.h" 

class Cube 
{
    private: 
        GLuint vao; 
        GLuint vbo_position; 
        GLuint vbo_color; 
        GLuint vbo_texCoord; 
        GLuint vbo_normal; 
        
    public: 
        Cube(); 
        int initialize(); 
        void render(); 
        ~Cube(); 
}; 

class Triangle 
{
    private: 
        GLuint vao; 
        GLuint vbo_position; 
        GLuint vbo_color; 
        GLuint vbo_texCoord; 
        GLuint vbo_normal; 

    public: 
        Triangle(); 
        int initialize(); 
        void render(); 
        ~Triangle(); 
}; 

class Quad 
{
    private: 
        GLuint vao; 
        GLuint vbo_position; 
        GLuint vbo_normal; 
        GLuint vbo_texCoord; 

    public: 
        Quad(); 
        int initialize(); 
        void render(); 
        ~Quad(); 
}; 

class Sphere 
{
    private: 
        GLuint vao; 
        GLuint vbo_position; 
        GLuint vbo_normal; 
        GLuint vbo_element; 

        float sphere_vertices[1146];
        float sphere_normals[1146];
        float sphere_textures[764];
        unsigned short sphere_elements[2280];
        int gNumElements, gNumVertices; 

    public: 
        Sphere(); 
        int initialize(); 
        void render(); 
        ~Sphere(); 
}; 

#endif /* _OBJECTS_HPP */ 
