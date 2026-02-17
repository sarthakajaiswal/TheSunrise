#pragma once 

#include "../global_header.hpp" 
#include "../shaderProgram.hpp" 
#include "../objects.hpp"
#include "../texture.hpp"
#include "../camera.hpp"
#include "../effects/terrain.hpp"

class Scene2 
{
    private:
        Terrain terrain; 
        const float terrainScale = 50.0f; 
        const int textureScale = 12; 
        const float heightScale = 12.0f; 

    public: 
        Scene2(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~Scene2(); 
}; 


