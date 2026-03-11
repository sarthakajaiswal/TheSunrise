#include "../../headers/scenes/scene2.hpp" 

Scene2::Scene2() 
{
    // code  
} 

Camera scene2Camera; 

float tx, ty, tz; 
float sx=1.0, sy=1.0, sz=1.0; 
float rx, ry, rz; 

int Scene2::initialize() 
{	
    // code 
    logFile.log("------------------ Scene2::initialize() started ----------------\n"); 

    std::vector<std::string> textureImages = {"res/terrain5.png", "res/terrain6.png", "res/terrain7.png", "res/terrain8.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/terr.png", 2.0f, 75.0f, textureImages, textureHeightRanges, textureScale);

    water.initialize(16.0); 

    scene2Camera.setState(vec3(452.49, 161.54, 593.75), 773.50, -3.90); 


    logFile.log("------------------ Scene2::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void Scene2::display() 
{
    mat4 modelMatrix = mat4::identity(); 
    viewMatrix = mat4::identity(); 
    viewMatrix = scene2Camera.getViewMatrix(CAMERA_GAME_MODE); 

    // In reflection and refraction all objects except water itself is rendered 
	glEnable(GL_CLIP_DISTANCE0); 

    GLfloat waterPosX = 963.26f; 
    GLfloat waterPosY = 28.75f; 
    GLfloat waterPosZ = 1045.70f; 

	// ************************** REFLECTION ************************** 
	water.reflectionFBO.bind(); 

    vec3 cameraPositionForReflection = scene2Camera.getPosition(); 

	// setting camera for reflection 
	float distanceFromWater = scene2Camera.getPosition()[1] - waterPosY; 
	cameraPositionForReflection[1] = cameraPositionForReflection[1] - (2*distanceFromWater); 
	
	vec3 cameraTarget = scene2Camera.getCenter();  
	float targetDistance = cameraTarget[1] - waterPosY; 
	cameraTarget[1] = cameraTarget[1] - 2.0f * targetDistance; 
	vmath::mat4 reflectionViewMatrix = vmath::lookat(cameraPositionForReflection, cameraTarget, vec3(0.0, 1.0, 0.0) ); 

	terrain.render(modelMatrix, reflectionViewMatrix, projectionMatrix, scene2Camera.getPosition()); 

	// cameraTarget = vec3(0.0, 0.0, 0.0); 
	// cameraPositionForReflection[1] = cameraPositionForReflection[1] - (2*distanceFromWater); 
	// viewMatrix = vmath::lookat(cameraPositionForReflection, cameraTarget, vec3(0.0, 1.0, 0.0)); 

	water.reflectionFBO.unbind(); 

	// // ************************** REFRACTION ************************** 
	water.refractionFBO.bind(); 
	terrain.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition()); 
	water.refractionFBO.unbind(); 

	glDisable(GL_CLIP_DISTANCE0); 

    /************** SCENE ****************/ 
    terrain.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition()); 

    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix = mat4::identity(); 
        // modelMatrix *= vmath::translate(963.26f, 28.75f, 1045.70f); 
        modelMatrix *= vmath::translate(waterPosX, waterPosY, waterPosZ); 
        modelMatrix *= vmath::scale(1000.0f, 1.0f, 1000.0f);

        water.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition(), vec3(1000.0), 4.0); 
    } 
    modelMatrix = matrixStack.popMatrix(); 
} 

void Scene2::update() 
{
    // code 
    water.update(); 
} 

Scene2::~Scene2() 
{
} 

void Scene2::eveneCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    // variable declarations 
    float cameraStep = 0.5f; 
    
    // code 
    bool doImGuiCapturedEvent = false; 
    if(ImGuiManager::initialized == true)
    {
        ImGuiIO& io = ImGui::GetIO(); 
        doImGuiCapturedEvent = io.WantCaptureMouse; 
    } 

    if(!doImGuiCapturedEvent) 
        scene2Camera.cameraCallback(hwnd, uMsg, wParam, lParam); 

    switch(uMsg) 
    {
        case WM_CHAR: 
            switch(wParam) 
            {
            default: 
                break; 
            } 
            break; 

        default: 
            break; 
    } 
} 


/* 
    modelMatrix = mat4::identity(); 
    modelMatrix *= vmath::translate(270.33f, 50.34f, 325.60f); 
    modelMatrix *= vmath::translate(tx, ty, tz); 
    modelMatrix *= vmath::rotate(rx, 1.0f, 0.0f, 0.0f);
    modelMatrix *= vmath::rotate(ry, 0.0f, 1.0f, 0.0f); 
    modelMatrix *= vmath::rotate(rz, 0.0f, 0.0f, 1.0f); 
    modelMatrix *= vmath::scale(sx, sy, sz);
*/ 