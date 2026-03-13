#pragma once 

#include <iostream> 
#include <vector> 
#include "global_header.hpp"
#include "common.hpp"

extern FileHandler logFile; 

class Slides 
{
    private: 
        FullScreenTexturer fsTexturer; 
        float fadeSpeed; 
        float textureDisplayTime; 
        std::vector<GLuint> textures; 
        
        size_t currentSlideNumber; 

    public: 
        Slides(); 
        int initialize(std::vector<const char*> textureImages, float fadeSpeed, float slideDisplayTime); 
        int render();  // returns 1 on all slide rendering got completed 
        void update(); 
        void uninitialize(); 
}; 
