#pragma once 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 
#include "../effects/exposure.hpp" 
#include "../effects/terrain.hpp" 
#include "../effects/cubemap.hpp" 
#include "../objects.hpp"
#include "../fbo.hpp" 

class Scene1 
{
    private:
        Exposure exposureProgram; 
        FullScreenTexturer fsTexturer;  
        Terrain terrain; 
        Cubemap cubemap; 

        FBO floatingPointFBO; 

        Quad quad; 

        GLuint quoteTexture; 
        GLuint gateTexture; 

    public: 
        float exposureValue; 

        Scene1(); 
        int initialize(); 
        void display(); 
        void update(); 
        void uninitialize(); 
        ~Scene1(); 

        void scene1Callbacks(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  
}; 


