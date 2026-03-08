#include "../headers/camera.hpp" 
#include "../headers/fileHandler.hpp"
extern FileHandler logFile; 

void Camera::updateVectorsAfterChangesInAngle(void) 
{
    direction[0] = cos(vmath::radians(yaw)) * cos(vmath::radians(pitch)); 
    direction[1] = sin(vmath::radians(pitch)); 
    direction[2] = sin(vmath::radians(yaw)) * cos(vmath::radians(pitch)); 
    
    up = vmath::vec3(0.0f, 1.0f, 0.0f); 
    front = normalize(direction); 
    right = normalize(cross(up, direction)); 
} 

Camera::Camera() 
            : positionSpline({vmath::vec3(0.0, 1.0, 1.0), vmath::vec3(0.0, 2.0, 2.0)}), 
                yawSpline({CAMERA_DEFAULT_YAW, CAMERA_DEFAULT_YAW}), 
                pitchSpline({CAMERA_DEFAULT_PITCH, CAMERA_DEFAULT_PITCH}) 
{
    position = vmath::vec3(0.0, 5.0, 3.0); 
    up = vmath::vec3(0.0, 1.0, 0.0); 
    target = vmath::vec3(0.0, 0.0, 0.0); 
    front = vmath::vec3(0.0, 0.0, -1.0); 
    
    direction = normalize(position - target); 
    yaw = CAMERA_DEFAULT_YAW; 
    pitch = CAMERA_DEFAULT_PITCH; 
    direction[0] = cos(vmath::radians(yaw)) * cos(vmath::radians(pitch)); 
    direction[1] = sin(vmath::radians(pitch)); 
    direction[2] = sin(vmath::radians(yaw)) * cos(vmath::radians(pitch)); 
    right = normalize(cross(up, direction)); 
    up = cross(direction, right);
} 

Camera::Camera(std::vector<vmath::vec3> _positionArray, std::vector<float> _yawArray, std::vector<float> _pitchArray) 
            : positionSpline(_positionArray), yawSpline(_yawArray), pitchSpline(_pitchArray) 
{
    this->positionArray = _positionArray; 
    this->yawArray = _yawArray; 
    this->pitchArray = _pitchArray; 
}  

vmath::vec3 Camera::getPosition()
{
    return (position); 
} 

vmath::vec3 Camera::getCenter() const 
{
    return (position+direction); 
} 

vmath::mat4 Camera::getViewMatrix(enum CameraMode mode, float t) 
{
    if(mode == CAMERA_GAME_MODE) 
    {
        vmath::mat4 viewMatrix = vmath::lookat(position, position+front, up); 
        return (viewMatrix); 
    } 
    else 
    {
        vmath::vec3 eye = positionSpline.evaluatePosition(t); 
        
        float yawAtT = yawSpline.evaluateValue(t); 
        float pitchAtT = pitchSpline.evaluateValue(t); 
        vmath::vec3 direction; 
        direction[0] = cos(vmath::radians(yawAtT)) * cos(vmath::radians(pitchAtT)); 
        direction[1] = sin(vmath::radians(pitchAtT)); 
        direction[2] = sin(vmath::radians(yawAtT)) * cos(vmath::radians(pitchAtT)); 
        vmath::vec3 center = eye + direction; 

        vmath::vec3 up = vmath::vec3(0.0, 1.0, 0.0); 

        logFile.log("t = %.2f => pos = %.2f, %.2f, %.2f yaw=%.2f pitch=%.2f center = %.2f, %.2f, %.2f up = %.2f, %.2f, %.2f\n", 
                t, eye[0], eye[1], eye[2], yawAtT, pitchAtT, center[0], center[1], center[2], up[0], up[1], up[2]); 

        vmath::mat4 viewMatrix = vmath::lookat(eye, center, up); 
        return (viewMatrix);  
    } 
} 

void Camera::setState(vmath::vec3 _position, float _yaw, float _pitch)
{
    position = _position; 
    yaw = _yaw; 
    pitch = _pitch; 
    updateVectorsAfterChangesInAngle(); 
} 

void setControlPoints(std::vector<vmath::vec3> positions, std::vector<float> yaws, std::vector<float> pitches) 
{

} 

void Camera::cameraCallback(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
    const float cameraSpeed = 2.8; 
    static BOOL bMouseDown = FALSE; 
    
    // mouse event related variables
    static BOOL bGameMode = FALSE; 
    float sensitivity = 0.1f; 

    static BOOL bJustModeChanged = TRUE; 
    static float lastX = 0.0; 
    static float lastY = 0.0; 
    float currentX, currentY; 
    float xOffset, yOffset; 

    switch(iMsg) 
    {
        case WM_LBUTTONDOWN: 
            lastX = GET_X_LPARAM(lParam); 
            lastY = GET_Y_LPARAM(lParam);
            bMouseDown = TRUE; 
            break; 

        case WM_LBUTTONUP: 
            bMouseDown = FALSE; 
            break; 

        case WM_MOUSEMOVE: 
            if(bGameMode == FALSE && bMouseDown == FALSE) 
                break; 

            if(bJustModeChanged) // get the mouse positions on changing the mode 
            {
                lastX = GET_X_LPARAM(lParam); 
                lastY = GET_Y_LPARAM(lParam);
                ShowCursor(!bGameMode); 
                
                bJustModeChanged = FALSE; 
            } 
            currentX = GET_X_LPARAM(lParam);
            currentY = GET_Y_LPARAM(lParam);
            xOffset = currentX - lastX; 
            yOffset = lastY - currentY; 
            lastX = currentX; 
            lastY = currentY; 

            xOffset *= sensitivity; 
            yOffset *= sensitivity; 

            yaw -= xOffset; 
            pitch -= yOffset; 

            if(pitch > 89.0f)
                pitch = 89.0f;
            if(pitch < -89.0f)
                pitch = -89.0f;
            updateVectorsAfterChangesInAngle(); 

            if(bGameMode == TRUE) 
            {
                RECT windowRect; 
                GetClientRect(ghwnd, &windowRect);
                lastX = windowRect.right/2;  
                lastY = windowRect.bottom/2;  
                // lastY = GetSystemMetrics(SM_CYSCREEN)/2; 
                SetCursorPos(lastX, lastY); 
            } 
            break; 

        case WM_CHAR: 
            switch(wParam) 
            {
            case 'w': 
            case 'W':   
                position += cameraSpeed * front; 
                break; 
            case 's': 
            case 'S': 
                position -= cameraSpeed * front; 
                break; 
            case 'a': 
            case 'A': 
                position -= normalize(cross(front, up)) * cameraSpeed; 
                break; 
            case 'd': 
            case 'D': 
                position += normalize(cross(front, up)) * cameraSpeed; 
                break; 

            case 'q': 
            case 'Q': 
                // position[1] -= sensitivity * 2.0; 
                position[1] -= cameraSpeed * 2.0; 
                break; 
            case 'e': 
            case 'E': 
                // position[1] += sensitivity * 2.0;
                position[1] += cameraSpeed * 2.0;
                break; 

            case 'g': 
            case 'G': 
                bGameMode = !bGameMode; 
                bJustModeChanged = true; // to save the values of lastX and lastY for firsttime after mode is changed 
                break; 

            case 'p': 
                printInfo(); 
                break; 
                
            default: 
                break; 
            } 
            break; 

        default: 
            break;  
    } 
} 

void Camera::printInfo() 
{
    logFile.log("%.2f, %.2f, %.2f | %.2f | %.2f\n", position[0], position[1], position[2], yaw, pitch); 
} 
