#pragma once 

#include "../global_header.hpp" 
#include "../shaderProgram.hpp" 
#include "../objects.hpp"
#include "../texture.hpp"

class Scene1 
{
    private:
        Quad quad; 

        ShaderProgram glowingPlanetProgram; 
        GLuint lightPositionUniform_glowingPlanet = 0; 
        GLuint resoulutionUniform_glowingPlanet = 0; 
        GLuint timeUniform_glowingPlanet = 0; 
        GLuint textureUniform_glowingPlanet = 0;

        GLuint texture_moon; 

        bool initGlowingPlanetProgram(); 

    public: 
        GLfloat lightPosition[4];

        Scene1(); 

        int initialize(); 
        void display(); 
        void update(); 
        
        ~Scene1(); 
}; 


