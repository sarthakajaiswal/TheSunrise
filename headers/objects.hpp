#ifndef _OBJECTS_HPP 
#define _OBJECTS_HPP 

#include "global_header.hpp"  

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
        void render(void); 
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
        void render(void); 
        ~Quad(); 
}; 

#endif /* _OBJECTS_HPP */ 
