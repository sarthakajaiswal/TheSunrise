#ifndef _TEST_SCENE 
#define _TEST_SCENE 

#include "../global_header.hpp" 
#include "../shaderProgram.hpp" 
#include "../objects.hpp" 
#include "../camera.hpp" 

class TestScene 
{
    private: 
        GLuint shaderProgramObject; 

    public: 
        TestScene(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~TestScene(); 
}; 


#endif /* _TEST_SCENE */ 
