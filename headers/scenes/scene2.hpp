#pragma once 

#include "../global_header.hpp" 
#include "../shaderProgram.hpp" 
#include "../objects.hpp"
#include "../camera.hpp"
#include "../imgui_manager.hpp" 
#include "../effects/terrain.hpp"
#include "../effects/water.hpp"

class Scene2 
{
    private:
        Terrain terrain; 
        const float terrainScale = 50.0f; 
        const int textureScale = 12; 
        const float heightScale = 12.0f; 

        Water water; 

    public: 
        Scene2(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~Scene2(); 

        void eveneCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
}; 


