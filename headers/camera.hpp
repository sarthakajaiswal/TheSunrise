#ifndef _CAMERA_HPP 
#define _CAMERA_HPP 

#include "global_header.hpp" 
#include "common.hpp" 

class Camera 
{
    private: 
        vec3 position; 
        vec3 front; 
        vec3 up; 

        vec3 direction; 
        float yaw; 
        float pitch; 

        float speed; 

    public: 
        Camera(); 
        ~Camera(); 

        vec3 getPosition() const; 

        void initialize(vec3 position, float yaw, float pitch); 
        mat4 getViewMatrix(); 
        void setSpeed(float newSpeed); 
        void updateCamera(); 

        void callbackFunction(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  
}; 

#endif /* _CAMERA_HPP */ 
