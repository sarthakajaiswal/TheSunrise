#include "../../headers/scenes/scene1.hpp" 

// float tx, ty, tz; 
// float sx=1.0, sy=1.0, sz=1.0; 
// float rx, ry, rz; 

static std::vector<vec3> path1ControlPoints = 
{ 
    {509.17, 92.07, 1111.14}, 
    {511.10, 90.38, 1151.05}, 
    {512.95, 89.08, 1184.97}, 
    {515.68, 87.96, 1230.88}, 
    {518.92, 88.74, 1294.79}, 
    {521.82, 90.75, 1358.69} 
}; 
static std::vector<float> path1Yaws = {91.20, 86.90, 86.60, 87.10, 87.40, 86.70 }; 
static std::vector<float> path1Pitches = { -3.70, -2.30, -1.40, 0.70, 1.80, 3.00 }; 

static std::vector<vec3> path2ControlPoints = 
{
    {521.82, 90.75, 1358.69}, 
    {515.82, 65.75, 1133.69}, 
    {510.13, 39.15, 908.28}, 
    {510.57, 28.12, 839.89}, 
    {509.16, 44.40, 748.99}, 
    {509.91, 50.93, 656.87} 
}; 
static std::vector<float> path2Yaws = {86.70, 87.70, 88.70, 88.30, 87.80, 84.10}; 
static std::vector<float> path2Pitches = { 3.00, 7.40, 12.40, 13.50, 11.60, 18.80 }; 

static float cameraSpeed = 0.0f; 
static float cameraT = 0.0f; 

float alpha=0.2; 
// initial 
float flareModelTx0 = 530.90f, flareModelTy0 = -121.92f, flareModelTz0 = 1709.79f; 
float flareModelSx0 = 3.64f, flareModelSy0 = 4.31f, flareModelSz0 = 5.36f;
// final 
float flareModelTx1 = 530.90f, flareModelTy1 = -6.03f, flareModelTz1 = 1544.01f; 
float flareModelSx1 = 30.74f, flareModelSy1 = 22.43f, flareModelSz1 = 15.0f; 
// current (initially set to initial values) 
float flareModelTx=flareModelTx0, flareModelTy=flareModelTy0, flareModelTz=flareModelTz0; 
float flareModelSx=flareModelSx0, flareModelSy=flareModelSy0, flareModelSz=flareModelSz0; 

Camera scene1Camera; 
float modelX = 534.06, modelY = 46.0, modelZ = 501.85; 
float modelSx = 1.0, modelSy = 1.0, modelSz = 1.0; 

static bool bHorrorLikeMusicStarted = false; // after enabling this godrays turns red and mindflare model starts to become bigger 
static float godraysRedComponent = 0.0; 
static bool bAutoCameraStart = false; 

static GLuint tex_dissolve; // TODO: texture should be loaded on server side 
static float dissolveValue = 0.0;

Scene1::Scene1() 
{
    // code  
    exposure_godrays = 0.179f; 
    decay_godrays = 0.92f; 
    density_godrays = 1.00f; 
    weight_godrays = 0.90f; 
    strength_godrays = 0.935f; 
    numSamples_godrays = 60; 
} 

float gateTextureExposureValue = 0.2f; 
static bool bShowQuoteTexture = true; 
static bool bShowGateTexture = true; 
static float quoteTextureAlpha = 0.0f; 
 
int Scene1::initialize() 
{	
    // code 
    logFile.log("------------------ Scene1::initialize() started ----------------\n"); 

    // -------- Effects --------- 
    // assert(exposureProgram.initialize() == 0); 
    assert(fsTexturer.initialize() == 0); 
    assert(textureBlender.initialize() == 0); 
    assert(exposureProgram.initialize() == 0); 
    assert(initBWShader() == 0); 

    std::vector<std::string> textureImages = {"res/terrain1.png", "res/terrain3.png", "res/terrain2.png", "res/terrain4.png"}; 
    std::vector<float> textureHeightRanges = {0.06, 0.4, 0.6, 1.0}; 
    terrain.initialize("res/scene1terrain.png", 1.0f, 60.0f, textureImages, textureHeightRanges, 4.0); 

    const char* cubemapImages[6] = 
    {
		"res/environments/environment1/px.png", 
		"res/environments/environment1/nx.png", 
		"res/environments/environment1/py.png", 
		"res/environments/environment1/ny.png", 
		"res/environments/environment1/pz.png", 
		"res/environments/environment1/nz.png" 
    }; 
    cubemap.initialize(cubemapImages); 

    godrays.initialize(); 

    // -------- Objects --------- 
    // assert(quad.initialize() == 0); 
    logFile.log("Loading Models...\n"); 
    assert(treeModel.initialize("res/models/game_tree/scene.gltf") == 0); 
    assert(mindFlare.initialize("res/models/mind_flayer/MindFlayer.glb") == 0); 
    logFile.log("Models Loaded\n"); 

    assert(moonSphere.initialize() == 0);

    // -------- FBO and textures ---------  
    assert(floatingPointFBO.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(sceneFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    gateTexture = loadTexture("res\\gate.png"); 
    if(gateTexture == 0) 
        throw texture_loading_failure("Scene1::initialize() > gate texture loading failed\n");
    tex_dissolve = loadTexture("res/dissolveTex.png"); 
    if(gateTexture == 0) 
        throw texture_loading_failure("Scene1::initialize() > dissolve texture loading failed\n");
    quoteTexture = loadTexture("res\\phrase1.png"); 
    if(quoteTexture == 0) 
        throw texture_loading_failure("Scene1::initialize() > phrase1 texture loading failed\n");

    scene1Camera.setState(vec3(521.82, 90.75, 1358.69), 90.60, 2.50); 

    // tx = scene1Camera.getPosition()[0]; 
    // ty = scene1Camera.getPosition()[1]; 
    // tz = scene1Camera.getPosition()[2]; 

    logFile.log("------------------ Scene1::initialize() completed ----------------\n\n"); 
    return (0); 
} 

void Scene1::display() 
{
    void resize(int, int); 
    glClearColor(0.0, 0.0, 0.0, 1.0); 
    glClear(GL_COLOR_BUFFER_BIT); 

    glViewport(0, 0, winWidth, winHeight); 
    resize(winWidth, winHeight); 

    mat4 modelMatrix = mat4::identity(); 
    if(bAutoCameraStart == true) 
        viewMatrix = scene1Camera.getViewMatrix(CAMERA_AUTO_MODE, cameraT); 
    // if(mainTimer > 55.0)
    //     viewMatrix = scene1Camera.getViewMatrix(CAMERA_GAME_MODE); 
    

    floatingPointFBO.bind(); 
    fsTexturer.render(gateTexture, 0.2); 
    floatingPointFBO.unbind(); 

    if(bShowQuoteTexture==true && bShowGateTexture==true) 
    {
        exposureProgram.render(floatingPointFBO.getTextureID(), gateTextureExposureValue); 
        glEnable(GL_BLEND); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        fsTexturer.render(quoteTexture, quoteTextureAlpha);
        glDisable(GL_BLEND);  
        return; // do not render other things
    }  
    else if(bShowGateTexture == true) 
    {
        exposureProgram.render(floatingPointFBO.getTextureID(), gateTextureExposureValue); 
        return; 
    } 

    sceneFBO.bind(); 
    {
        // terrain 
        matrixStack.pushMatrix(modelMatrix); 
        {
            terrain.render(mat4::identity(), viewMatrix, projectionMatrix, scene1Camera.getPosition(), true, true, vec3(50.0), vec3(1.0*godraysRedComponent, 0.0, 0.0), 150.0, 250.0, vec3(0.0, 0.0, 0.0)); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

        // tree 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix = mat4::identity(); 
            modelMatrix *= vmath::translate(660.032f, 27.08f, 948.01f); 
            modelMatrix *= vmath::scale(0.189f, 0.199f, 0.224f);
            treeModel.render(modelMatrix, viewMatrix, projectionMatrix, true, 150.0, 250.0, vec3(0.0, 0.0, 0.0), scene1Camera.getPosition()); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

        // moon 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix = mat4::identity(); 

            vmath::mat4 translationMatrix = vmath::translate(490.6f, 455.627f, 2000.0f); 
            vmath::mat4 scaleMatrix = vmath::scale(125.0f, 125.0f, 125.0f); 
            modelMatrix = translationMatrix * scaleMatrix; 

            bwShader.use(); 
            glUniformMatrix4fv(mvpMatrixUniform_bwShader, 1, GL_FALSE, projectionMatrix*viewMatrix*modelMatrix); 
            moonSphere.render(); 
            bwShader.unuse(); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

        // mind flayer 
        if(bHorrorLikeMusicStarted == true) 
        {
            matrixStack.pushMatrix(modelMatrix); 
            {
                modelMatrix = mat4::identity(); 
                modelMatrix *= vmath::translate(flareModelTx, flareModelTy, flareModelTz); 
                modelMatrix *= vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
                modelMatrix *= vmath::scale(flareModelSx, flareModelSy, flareModelSz); 

                mindFlare.render(modelMatrix, viewMatrix, projectionMatrix, true, 150.0, 400.0, vec3(0.0, 0.0, 0.0), scene1Camera.getPosition(), true, tex_dissolve, dissolveValue); 
            } 
            modelMatrix = matrixStack.popMatrix(); 
        } 

        // cubemap  
        matrixStack.pushMatrix(modelMatrix); 
        {
            mat4 rotationMatrix = vmath::rotate(162.0f, 1.0f, 0.0f, 0.0f); 
            rotationMatrix *= vmath::rotate(0.6f, 0.0f, 1.0f, 0.0f); 
            rotationMatrix *= vmath::rotate(7.11f, 0.0f, 0.0f, 1.0f); 
            modelMatrix = rotationMatrix; 
            modelMatrix *= vmath::scale(vec3(10.0)); 

            cubemap.render(modelMatrix, viewMatrix, projectionMatrix); 
        } 
        modelMatrix = matrixStack.popMatrix(); 
    } 
    sceneFBO.unbind(); 

    /*********** GODRAYS ************/ 
    godrays.occlusionFBO.bind(); 
    { 
        glClearColor(godraysRedComponent, 0.0, 0.0, 1.0); 
        glClear(GL_COLOR_BUFFER_BIT); 

        // terrain 
        matrixStack.pushMatrix(modelMatrix); 
        {
            terrain.renderOcclusion(mat4::identity(), viewMatrix, projectionMatrix); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

        // tree 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix *= vmath::translate(660.032f, 27.08f, 948.01f); 
            modelMatrix *= vmath::scale(0.189f, 0.199f, 0.224f);
            treeModel.renderOcclusion(modelMatrix, viewMatrix, projectionMatrix); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

        // mind flayer 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix = mat4::identity(); 
            modelMatrix *= vmath::translate(flareModelTx, flareModelTy, flareModelTz); 
            modelMatrix *= vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
            modelMatrix *= vmath::scale(flareModelSx, flareModelSy, flareModelSz); 

            mindFlare.renderOcclusion(modelMatrix, viewMatrix, projectionMatrix, true, tex_dissolve, dissolveValue); 
        } 
        modelMatrix = matrixStack.popMatrix();

        // moon 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix = mat4::identity(); 

            vmath::mat4 translationMatrix = vmath::translate(490.6f, 455.627f, 2000.0f); 
            vmath::mat4 scaleMatrix = vmath::scale(125.0f, 125.0f, 125.0f); 
            modelMatrix = translationMatrix * scaleMatrix; 

            bwShader.use(); 
            glUniformMatrix4fv(mvpMatrixUniform_bwShader, 1, GL_FALSE, projectionMatrix*viewMatrix*modelMatrix); 
            moonSphere.render(); 
            bwShader.unuse(); 
        } 
        modelMatrix = matrixStack.popMatrix(); 
    } 
    godrays.occlusionFBO.unbind(); 

    godrays.renderWithScene(
                sceneFBO.getTextureID(), 
                viewMatrix, projectionMatrix, 
                exposure_godrays, decay_godrays, density_godrays, weight_godrays, strength_godrays, numSamples_godrays, vmath::vec3(490.6f, 455.627f, 2000.0f)
            ); 

    // if(bShowFadeOutQuad == true) 
    // {
    //     glEnable(GL_BLEND); 
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    //     fsTexturer.render(0, fadeOutAlpha); 
    //     glDisable(GL_BLEND); 
    // } 

    // glViewport(1200, 600, 400, 250);    
    // fsTexturer.render(sceneFBO.getTextureID()); 
} 

void Scene1::update() 
{
    // code 
    // model scale and translate alpha
    static float modelAnimateSpeedInverse = 300.0f; 
    float modelTxD=(flareModelTx1-flareModelTx0)/modelAnimateSpeedInverse;
    float modelTyD=(flareModelTy1-flareModelTy0)/modelAnimateSpeedInverse;
    float modelTzD=(flareModelTz1-flareModelTz0)/modelAnimateSpeedInverse; 
    float modelSxD=(flareModelSx1-flareModelSx0)/modelAnimateSpeedInverse;
    float modelSyD=(flareModelSy1-flareModelSy0)/modelAnimateSpeedInverse; 
    float modelSzD=(flareModelSz1-flareModelSz0)/modelAnimateSpeedInverse; 

    if(bHorrorLikeMusicStarted == true) 
    {
        if(flareModelSy <= flareModelSy1) 
        {
            flareModelTx += modelTxD; 
            flareModelTy += modelTyD; 
            flareModelTz += modelTzD; 
            flareModelSx += modelSxD; 
            flareModelSy += modelSyD; 
            flareModelSz += modelSzD; 
        } 

        if(godraysRedComponent < 0.05) 
            godraysRedComponent += 0.0002; 
    } 

    // quote texture 
    if(bShowQuoteTexture == true && quoteTextureAlpha < 1.0) 
        quoteTextureAlpha += 0.004; 
    if(mainTimer > 40.0) 
    {
        quoteTextureAlpha -= 0.01; 
        if(quoteTextureAlpha < 0.0f) 
            bShowQuoteTexture = false; 
    } 
    if(mainTimer > 42.0 && bShowQuoteTexture == true) 
        bShowQuoteTexture = false; 

    // gate texture 
    static float angle=0.0f;  
    if(mainTimer < 43.0f) 
    {
        gateTextureExposureValue = 2.5*(1.0+sin(radians(angle))); 
        angle += 0.4f; 
    } 
    if(mainTimer > 43.0 && gateTextureExposureValue > 0.0) 
    {
        gateTextureExposureValue = 2.5*(1.0+sin(radians(angle))); 
        angle += 0.4f; 
    } 
    
    if(mainTimer > 45.0 && mainTimer <= 55.0f) 
    {
        static bool isFirstTime = true; 
        
        if(isFirstTime) 
        {
            bShowGateTexture = false; 
            bAutoCameraStart = true; 
            cameraSpeed = 0.002f; 
            scene1Camera.automise(path1ControlPoints, path1Yaws, path1Pitches); 
            isFirstTime = false; 
        } 

        if(cameraT < 1.0) 
        {
            cameraT += cameraSpeed; 
            if(cameraT > 1.0f) 
                cameraT = 1.0;  
        } 
    } 
    else if(mainTimer > 55.0 && mainTimer <= 69.0) 
    {
        static bool isFirstTime = true; 
        
        if(isFirstTime) 
        {
            bHorrorLikeMusicStarted = true;  
            cameraT = 0.0; 
            cameraSpeed = 0.002f; 
            scene1Camera.automise(path2ControlPoints, path2Yaws, path2Pitches); 
            isFirstTime = false; 
        }    

        if(strength_godrays < 2.0) 
            strength_godrays += 0.002f; 

        if(cameraT < 1.0) 
        {
            cameraT += cameraSpeed; 
            if(cameraT > 1.0f) 
                cameraT = 1.0;  
        } 
    } 
    else if(mainTimer > 72.0) 
    {
        static bool isFirstTime = true; 
        if(isFirstTime) 
        {
            dissolveValue = 0.1f; 
            isFirstTime = false; 
        }   

        if(mainTimer > 74.0) 
        {
            dissolveValue += 0.01f; 
        } 

        if(dissolveValue >= 1.2) 
        {
            CurrentScene = SCENE_2; 
        } 
    } 
} 

void Scene1::uninitialize()  
{
    // code 
    logFile.log("Scene1::uninitialize() > Uninitializing...\n"); 
    if(gateTexture != 0) 
    {
        glDeleteTextures(1, &gateTexture); 
        gateTexture = 0; 
    } 

    if(quoteTexture != 0) 
    {
        glDeleteTextures(1, &quoteTexture); 
        quoteTexture = 0; 
    } 
         
    sceneFBO.destroyFBO(); 
    floatingPointFBO.destroyFBO(); 
    textureBlender.uninitialize();      
    cubemap.uninitialize(); 
    godrays.uninitialize(); 
    terrain.uninitialize(); 

    logFile.log("Scene1::uninitialize() > Uninitialized\n"); 
} 

Scene1::~Scene1() 
{
} 

int Scene1::initBWShader() 
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 

    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/bw.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/bw.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    
    bwShader.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform_bwShader = bwShader.getUniformLocation("uMVPMatrix"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
    
    return (0); 
} 

void Scene1::eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    bool doImGuiCapturedEvent = false; 
    if(ImGuiManager::initialized == true)
    {
        ImGuiIO& io = ImGui::GetIO(); 
        doImGuiCapturedEvent = io.WantCaptureMouse; 
    } 

    if(!doImGuiCapturedEvent) 
        scene1Camera.cameraCallback(hwnd, uMsg, wParam, lParam);  

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
        modelMatrix *= vmath::translate(tx, ty, tz); 
        modelMatrix *= vmath::rotate(rx, 1.0f, 0.0f, 0.0f);
        modelMatrix *= vmath::rotate(ry, 0.0f, 1.0f, 0.0f); 
        modelMatrix *= vmath::rotate(rz, 0.0f, 0.0f, 1.0f); 
        modelMatrix *= vmath::scale(sx, sy, sz);
*/ 

/* 
        // mind flayer 
        matrixStack.pushMatrix(modelMatrix); 
        {
            modelMatrix = mat4::identity(); 
            modelMatrix *= vmath::translate(flareModelTx, flareModelTy, flareModelTz); 
            modelMatrix *= vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
            modelMatrix *= vmath::scale(flareModelSx, flareModelSy, flareModelSz); 

            mindFlare.renderOcclusion(modelMatrix, viewMatrix, projectionMatrix); 
        } 
        modelMatrix = matrixStack.popMatrix(); 

*/ 