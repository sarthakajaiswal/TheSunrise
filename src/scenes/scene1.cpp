#include "../../headers/scenes/scene1.hpp" 

// float tx, ty, tz; 
// float sx=1.0, sy=1.0, sz=1.0; 
// float rx, ry, rz; 

float alpha=0.2; 

// initial 
float flareModelTx0 = 530.90f, flareModelTy0 = -121.92f, flareModelTz0 = 1709.79f; 
float flareModelSx0 = 3.64f, flareModelSy0 = 4.31f, flareModelSz0 = 5.36f;
// final 
float flareModelTx1 = 530.90f, flareModelTy1 = -6.03f, flareModelTz1 = 1544.01f; 
float flareModelSx1 = 30.74f, flareModelSy1 = 22.43f, flareModelSz1 = 5.0f; 
// current (initially set to initial values) 
float flareModelTx=flareModelTx0, flareModelTy=flareModelTy0, flareModelTz=flareModelTz0; 
float flareModelSx=flareModelSx0, flareModelSy=flareModelSy0, flareModelSz=flareModelSz0; 
bool bAnimateFlareModel = true; 

Camera scene1Camera; 
float modelX = 534.06, modelY = 46.0, modelZ = 501.85; 
float modelSx = 1.0, modelSy = 1.0, modelSz = 1.0; 

Scene1::Scene1() 
{
    // code  
    exposureValue = 0.2f; 

    exposure_godrays = 0.34f; 
    decay_godrays = 0.96f; 
    density_godrays = 0.84f; 
    weight_godrays = 0.58f; 
    numSamples_godrays = 50; 
} 

int Scene1::initialize() 
{	
    // code 
    logFile.log("------------------ Scene1::initialize() started ----------------\n"); 

    // -------- Effects --------- 
    // assert(exposureProgram.initialize() == 0); 
    assert(fsTexturer.initialize() == 0); 
    assert(textureBlender.initialize() == 0); 
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
    assert(mindFlare.initialize("res/models/mind_flayer/mind_flayer.obj") == 0); 
    logFile.log("Models Loaded\n"); 

    assert(moonSphere.initialize() == 0);

    // -------- FBO and textures --------- 
    // assert(floatingPointFBO.createFloatingPointFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  
    assert(testFBO.createNormalFBO(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)) == true);  

    gateTexture = loadTexture("res\\gate.png"); 
    if(gateTexture == 0) 
        throw texture_loading_failure("Scene1::initialize() > gate texture loading failed\n");
        
    // quoteTexture = loadTexture("res\\phrase1.png"); 
    // if(quoteTexture == 0) 
    //     throw texture_loading_failure("Scene1::initialize() > phrase1 texture loading failed\n");

    scene1Camera.setState(vec3(534.06, 46.83, 501.85), 88.60, 1.20); 

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
    viewMatrix = scene1Camera.getViewMatrix(CAMERA_GAME_MODE); 
    // viewMatrix = mat4::identity(); 

    // floatingPointFBO.bind(); 
    // fsTexturer.render(gateTexture); 
    // floatingPointFBO.unbind(); 

    // exposureProgram.render(floatingPointFBO.getTextureID(), exposureValue); 

    // glEnable(GL_BLEND); 
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    // fsTexturer.render(quoteTexture); 
    // glDisable(GL_BLEND); 

    // ---------- 

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

    // terrain 
    matrixStack.pushMatrix(modelMatrix); 
    {
        terrain.render(mat4::identity(), viewMatrix, projectionMatrix, scene1Camera.getPosition(), true, false, vec3(50.0), vec3(1.0, 0.0, 0.0)); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    // tree 
    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix = mat4::identity(); 
        modelMatrix *= vmath::translate(660.032f, 27.08f, 948.01f); 
        modelMatrix *= vmath::scale(0.189f, 0.199f, 0.224f);
        treeModel.draw(modelMatrix, viewMatrix, projectionMatrix); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    // mind flayer 
    matrixStack.pushMatrix(modelMatrix); 
    {
        modelMatrix = mat4::identity(); 
        modelMatrix *= vmath::translate(flareModelTx, flareModelTy, flareModelTz); 
        modelMatrix *= vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
        modelMatrix *= vmath::scale(flareModelSx, flareModelSy, flareModelSz); 

        mindFlare.draw(modelMatrix, viewMatrix, projectionMatrix); 
    } 
    modelMatrix = matrixStack.popMatrix(); 

    // /*********** GODRAYS ************/ 
    // godrays.sceneObjectsFBO.bind(); 
    // { 
    //     // terrain 
    //     matrixStack.pushMatrix(modelMatrix); 
    //     {
    //         terrain.render(mat4::identity(), viewMatrix, projectionMatrix, scene1Camera.getPosition()); 
    //     } 
    //     modelMatrix = matrixStack.popMatrix(); 

    //     // tree 
    //     matrixStack.pushMatrix(modelMatrix); 
    //     {
    //         modelMatrix *= vmath::translate(660.032f, 27.08f, 948.01f); 
    //         modelMatrix *= vmath::scale(0.189f, 0.199f, 0.224f);
    //         treeModel.draw(modelMatrix, viewMatrix, projectionMatrix); 
    //     } 
    //     modelMatrix = matrixStack.popMatrix(); 

    //     // mind flayer 
    //     matrixStack.pushMatrix(modelMatrix); 
    //     {
    //         modelMatrix = mat4::identity(); 
    //         modelMatrix *= vmath::translate(flareModelTx, flareModelTy, flareModelTz); 
    //         modelMatrix *= vmath::rotate(180.0f, 0.0f, 1.0f, 0.0f);
    //         modelMatrix *= vmath::scale(flareModelSx, flareModelSy, flareModelSz); 

    //         mindFlare.draw(modelMatrix, viewMatrix, projectionMatrix); 
    //     } 
    //     modelMatrix = matrixStack.popMatrix(); 

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
    // } 
    // godrays.sceneObjectsFBO.unbind(); 

    // godrays.lightSourceFBO.bind(); 
    // { 
    //     matrixStack.pushMatrix(modelMatrix); 
    //     {
    //         modelMatrix = mat4::identity(); 

    //         vmath::mat4 translationMatrix = vmath::translate(490.6f, 455.627f, 2000.0f); 
    //         vmath::mat4 scaleMatrix = vmath::scale(125.0f, 125.0f, 125.0f); 
    //         modelMatrix = translationMatrix * scaleMatrix; 

    //         bwShader.use(); 
    //         glUniformMatrix4fv(mvpMatrixUniform_bwShader, 1, GL_FALSE, projectionMatrix*viewMatrix*modelMatrix); 
    //         moonSphere.render(); 
    //         bwShader.unuse(); 
    //     } 
    //     modelMatrix = matrixStack.popMatrix(); 
    // } 
    // godrays.lightSourceFBO.unbind(); 

    // matrixStack.pushMatrix(modelMatrix);
    // { 
    //     mat4 mvpMatrix = projectionMatrix * viewMatrix * modelMatrix; 
    //     glEnable(GL_BLEND); 
    //     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    //     godrays.render(
    //         viewMatrix, projectionMatrix, exposure_godrays, decay_godrays, density_godrays, weight_godrays, numSamples_godrays, vec3(490.6f, 455.627f, 2000.0f)); 
    //     glDisable(GL_BLEND); 
    // } 
    // modelMatrix = matrixStack.popMatrix(); 


    /*****************************************************************************************************************************/

    // void resize(int, int); 
    // resize(winWidth, winHeight); 

    // testFBO.bind(); 
    // glEnable(GL_BLEND); 
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    // fsTexturer.render(godrays.sceneObjectsFBO.getTextureID()); 
    // glDisable(GL_BLEND); 
    // testFBO.unbind(); 

    // GLuint motionBlurTexture = godrays.createMotionBlurTexture(exposure_godrays, decay_godrays, density_godrays, weight_godrays, numSamples_godrays, vec2(0.5, 0.5)); 
    // GLuint occlusionTexture = godrays.createOcclusionTexture(); 
    // GLuint sceneTexture = godrays.createSceneTexture(); 
    // GLuint finalCompositeTexture = godrays.getFinalCompositeTexture(); 

    // GLuint finalCompositeTexture = godrays.render(
    //         viewMatrix, projectionMatrix, exposure_godrays, decay_godrays, density_godrays, weight_godrays, numSamples_godrays, vec3(490.6f, 455.627f, 2000.0f)); 

    // fsTexturer.render(godrays.getFinalCompositeTexture()); 

    // glViewport(10, 600, 400, 250);    
    // glEnable(GL_BLEND); 
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    // fsTexturer.render(gateTexture); 
    // glDisable(GL_BLEND);
    // fsTexturer.render(motionBlurTexture); 
    // fsTexturer.render(sceneTexture); 
    // fsTexturer.render(occlusionTexture); 
    // fsTexturer.render(gateTexture); 
} 

void Scene1::update() 
{
    // code 
    // model scale and translate alpha
    static float modelAnimateSpeedInverse = 100.0f; 
    float modelTxD=(flareModelTx1-flareModelTx0)/modelAnimateSpeedInverse;
    float modelTyD=(flareModelTy1-flareModelTy0)/modelAnimateSpeedInverse;
    float modelTzD=(flareModelTz1-flareModelTz0)/modelAnimateSpeedInverse; 
    float modelSxD=(flareModelSx1-flareModelSx0)/modelAnimateSpeedInverse;
    float modelSyD=(flareModelSy1-flareModelSy0)/modelAnimateSpeedInverse; 
    float modelSzD=(flareModelSz1-flareModelSz0)/modelAnimateSpeedInverse; 

    if(bAnimateFlareModel == true && flareModelSy <= flareModelSy1) 
    {
        flareModelTx += modelTxD; 
        flareModelTy += modelTyD; 
        flareModelTz += modelTzD; 
        flareModelSx += modelSxD; 
        flareModelSy += modelSyD; 
        flareModelSz += modelSzD; 
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

