#ifndef CAMERA_HPP 
#define CAMERA_HPP 

#include <Windowsx.h> 

#include "OGL.hpp" 
extern HWND ghwnd; 

class Camera 
{
    private: 
        vec3 position; 
        vec3 up; 
        vec3 target; 
        vec3 direction; 

        vec3 right; 
        vec3 front; 

        float yaw, pitch; 

        void updateVectorsAfterChangesInAngle(void); 
        
    public: 
        Camera(); 
        void setPosition(vec3 newPosition); 
        // void setCenter(vec3 center); 
        mat4 getViewMatrix(); 
        void cameraCallback(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam); 
        void printInfo(); 
}; 

#endif // CAMERA_HPP 

