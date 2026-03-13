#pragma once  

#include "../global_header.hpp" 
#include "../slides.hpp" 
#include "../common.hpp" 

class OutroScene 
{
    private: 
        Slides slidesSequence;
        FullScreenTexturer fsTexturer; 
        GLuint texture_theSystemCallsPoster; 

    public: 
        OutroScene(); 

        int initialize(); 
        void display(); 
        void update(); 
        void uninitialize(); 

        void eveneCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

        ~OutroScene(); 
}; 

