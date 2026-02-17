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
        const float terrainScale = 6.0f; 
        const int textureScale = 8; 
        const float heightScale = 2.0f; 

    public: 
        Scene2(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~Scene2(); 
}; 


