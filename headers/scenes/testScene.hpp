#ifndef _TEST_SCENE 
#define _TEST_SCENE 

#include "../global_header.hpp" 
#include "../shaderProgram.hpp" 
#include "../objects.hpp" 
#include "../camera.hpp" 
#include "../spline.hpp" 
#include "../imgui_manager.hpp" 

class TestScene 
{
    private: 
        ShaderProgram shaderProgramObject; 
        GLuint mvpMatrixUniform; 

        Cube cube; 

        bool initShaderProgram(); 

    public: 
        TestScene(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~TestScene(); 

        void eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
}; 


#endif /* _TEST_SCENE */ 
