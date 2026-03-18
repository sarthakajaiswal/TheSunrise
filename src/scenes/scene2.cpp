#include "../../headers/scenes/scene2.hpp" 

Scene2::Scene2() 
{
    // code  
} 

Camera scene2Camera; 

float tx, ty, tz; 
float sx=1.0, sy=1.0, sz=1.0; 
float rx, ry, rz; 

static bool bShowFade = true; 
static float fadeAlpha = 1.0f;
static bool bShowQuoteTexture = false; 
static float quoteTextureAlpha = 0.0f; 

int Scene2::initialize() 
{	
    // code 
    logFile.log("------------------ Scene2::initialize() started ----------------\n"); 

    std::vector<std::string> textureImages = {"res/terrain5.png", "res/terrain6.png", "res/terrain8.png", "res/terrain7.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/terr.png", 2.0f, 75.0f, textureImages, textureHeightRanges, textureScale);

    water.initialize(16.0); 
    fsTexturer.initialize(); 

    const char* cubemapImages[6] = 
    {
		"res/environments/environment2-sunrise/px.png", 
		"res/environments/environment2-sunrise/nx.png", 
		"res/environments/environment2-sunrise/py.png", 
		"res/environments/environment2-sunrise/ny.png", 
		"res/environments/environment2-sunrise/pz.png", 
		"res/environments/environment2-sunrise/nz.png" 
    }; 
    cubemap.initialize(cubemapImages); 

    phraseTexture = loadTexture("res/phrase2.png"); 
    if(phraseTexture == 0) 
        throw texture_loading_failure("failed to load phrase2 texture"); 

    flowerModel.initialize("res/models/flower_shrub/scene.gltf"); 
    butterfly.initialize("res/butterflyWing.png"); 

    scene2Camera.setState(vec3(1059.29, 85.05, 1481.44), 1040.70, -9.80); 

    logFile.log("------------------ Scene2::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void Scene2::display() 
{
    mat4 modelMatrix = mat4::identity(); 
    viewMatrix = mat4::identity(); 
    viewMatrix = scene2Camera.getViewMatrix(CAMERA_GAME_MODE); 

    // // In reflection and refraction all objects except water itself is rendered 
	glEnable(GL_CLIP_DISTANCE0); 

    GLfloat waterPosX = 963.26f; 
    GLfloat waterPosY = 28.75f+ty; 
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
	vmath::mat4 reflectionViewMatrix = vmath::lookat(cameraPositionForReflection, cameraTarget, vec3(0.0, 1.0, 0.0)); 

	terrain.render(modelMatrix, reflectionViewMatrix, projectionMatrix, scene2Camera.getPosition(), vec4(0.0, 1.0, 0.0, -waterPosY)); 

	water.reflectionFBO.unbind(); 

	// ************************** REFRACTION ************************** 
	water.refractionFBO.bind(); 
	terrain.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition(), vec4(0.0, -1.0, 0.0, waterPosY)); 
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

        water.render(modelMatrix, viewMatrix, projectionMatrix, scene2Camera.getPosition(), vec3(1000.0), 1.0); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    // flower tree 
    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix = mat4::identity(); 
        modelMatrix *= vmath::translate(1100.50f, 52.0f, 1377.0f); 
        modelMatrix *= vmath::scale(0.184f, 0.187f, 0.219f);
        flowerModel.render(modelMatrix, viewMatrix, projectionMatrix); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    // butterfly 
    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix = mat4::identity(); 
        modelMatrix *= vmath::translate(1090.214f, 76.452f, 1410.285f); 
        modelMatrix *= vmath::rotate(324.0f, 0.0f, 0.0f);
        modelMatrix *= vmath::scale(1.903f, 1.75f, 1.089f);
        butterfly.render(modelMatrix, viewMatrix, projectionMatrix); 
        
        modelMatrix *= vmath::translate(-3.059f, 3.211f, -5.60f); 
        modelMatrix *= vmath::rotate(0.0f, 0.0f, 316.69f);
        modelMatrix *= vmath::scale(1.968f, 1.0f, 1.0f);
        butterfly.render(modelMatrix, viewMatrix, projectionMatrix); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    if(bShowQuoteTexture == true) 
    {
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        fsTexturer.render(phraseTexture, quoteTextureAlpha); 
        glDisable(GL_BLEND);         
    } 

    // rendering black quad for fade effect 
    if(bShowFade) 
    {
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        fsTexturer.render(0, fadeAlpha); 
        glDisable(GL_BLEND); 
    } 

    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix *= vmath::rotate(100.0f, 0.0f, 1.0f, 0.0f); 
        modelMatrix *= vmath::rotate(rz, 0.0f, 0.0f, 1.0f); 
        cubemap.render(modelMatrix, viewMatrix, projectionMatrix); 
    } 
    modelMatrix = matrixStack.popMatrix(); 
} 

void Scene2::update() 
{
    // code 
    water.update(); 
    butterfly.update(4.0); 

    if(bShowFade == true) 
    {
        fadeAlpha -= 0.0006f; 
        if(fadeAlpha < 0.0f) 
            bShowFade = false; 
    } 

    if(mainTimer > 120.0) 
    {
        bShowQuoteTexture = true; 

        const float MAX_ALPHA = 2.0; // increase this to make phrase texture last longer 
        static bool pingpong = true; 
        if(quoteTextureAlpha < MAX_ALPHA && pingpong == true) 
        {
            quoteTextureAlpha += 0.01f; 
            if(quoteTextureAlpha >= MAX_ALPHA) 
                pingpong = false; 
        } 
        else if(quoteTextureAlpha > 0.0)  
        {
            quoteTextureAlpha -= 0.01f; 
            if(quoteTextureAlpha <= 0.0) 
                bShowQuoteTexture = false; 
        } 
    } 

    if(mainTimer > 130.0) 
    {
        bShowFade = true; 
        if(fadeAlpha < 1.0) 
            fadeAlpha += 0.01f; 
        else 
            CurrentScene = OUTRO_SCENE; 
    } 
} 

void Scene2::uninitialize() 
{	
    // code 
    logFile.log("------------------ Scene2::uninitialize() started ----------------\n"); 

    terrain.uninitialize(); 
    water.uninitialize(); 
    cubemap.uninitialize(); 

    phraseTexture = loadTexture("res/phrase2.png"); 
    if(phraseTexture == 0) 
        throw texture_loading_failure("failed to load phrase2 texture"); 

    flowerModel.initialize("res/models/flower_shrub/scene.gltf"); 
    butterfly.initialize("res/butterflyWing.png"); 

    scene2Camera.setState(vec3(1059.29, 85.05, 1481.44), 1040.70, -9.80); 

    logFile.log("------------------ Scene2::uninitialize() completed ----------------\n\n"); 
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