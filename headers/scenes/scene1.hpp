#pragma once 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 
#include "../effects/exposure.hpp" 
#include "../effects/terrain.hpp" 
#include "../effects/cubemap.hpp" 
#include "../effects/godrays.hpp" 
#include "../objects.hpp"
#include "../assimpModelLoader.hpp"
#include "../fbo.hpp" 
#include "../../ImGui/imgui.h" 
#include "../imgui_manager.hpp" 

class Scene1 
{
    private:
        Exposure exposureProgram; 
        FullScreenTexturer fsTexturer;  
        Terrain terrain; 
        Godrays godrays; 
        Cubemap cubemap; 
        
        Model treeModel; 
        Quad quad; 

        FBO floatingPointFBO; 
        FBO testFBO; 

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


