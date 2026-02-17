#include "../headers/camera.hpp" 

// TODO: 10.4] Movement Speed : same speed across machines 
Camera::Camera() 
{
    this->position = vec3(0.0, 0.0, 5.0); 
    this->front = vec3(0.0, 0.0, -1.0); 
    this->up = vec3(0.0, 1.0, 0.0); 

    this->speed = 1.0; 

    this->yaw = -90.0f; 
    this->pitch = 0.0f; 

    updateCamera(); 
} 

Camera::~Camera() 
{} 
 
vec3 Camera::getPosition() const 
{
    return (this->position); 
} 

void Camera::setSpeed(float newSpeed) 
{
    speed = newSpeed; 
} 

mat4 Camera::getViewMatrix() 
{
    mat4 viewMatrix = vmath::lookat(
                        position, 
                        position + front, 
                        up
                    ); 
    return (viewMatrix); 
} 

void Camera::initialize(vec3 position, float yaw, float pitch)  
{
    this->position = position; 
    this->yaw += yaw; 
    this->pitch = pitch; 
} 

void Camera::updateCamera() 
{
    float yawInRad = degToRad(yaw); 
    float pitchInRad = degToRad(pitch); 

    direction[0] = cos(yawInRad) * cos(pitchInRad); 
    direction[1] = sin(pitchInRad); 
    direction[2] = sin(yawInRad) * cos(pitchInRad);

    front = normalize(direction); 
} 

void Camera::callbackFunction(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    // code 
    MOUSEMOVEPOINT mouseInfo; 
    switch(uMsg) 
    {
        case WM_CHAR: 
            switch(wParam) 
            {
                case 'w': 
                case 'W':  
                    position += speed * front; 
                    break; 

                case 'S': 
                case 's':  
                    position -= speed * front; 
                    break; 

                case 'A': 
                case 'a':  
                    position -= normalize(vmath::cross(front, up)); 
                    break; 

                case 'd': 
                case 'D':  
                    position += normalize(vmath::cross(front, up)); 
                    break; 

                case '4': 
                    yaw -= 1.0; 
                    break; 

                case '6': 
                    yaw += 1.0f; 
                    break; 

                case '2': 
                    pitch -= 1.0; 
                    break; 

                case '8': 
                    pitch += 1.0f; 
                    break; 

                default: 
                    break; 
            } 
            break; 

        default: 
            break; 
    } 

    updateCamera(); 
} 
