#include "../../headers/scenes/testScene.hpp" 

Camera testSceneCamera; 
Spline3D spline; 

TestScene::TestScene() 
{
    // code  
} 

int TestScene::initialize() 
{	
    assert(initShaderProgram() == true); 
    cube.initialize(); 

    return (0); 
} 

void TestScene::display() 
{
    mat4 modelMatrix = mat4::identity(); 
    mat4 viewMatrix = mat4::identity(); 
    viewMatrix = testSceneCamera.getViewMatrix(CAMERA_GAME_MODE); 

    spline.show(projectionMatrix*viewMatrix*modelMatrix); 

    shaderProgramObject.use(); 
    glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, projectionMatrix*viewMatrix*modelMatrix); 
    cube.render(); 
    shaderProgramObject.unuse(); 
} 

void TestScene::update() 
{
    // code 
} 

TestScene::~TestScene() 
{
} 

bool TestScene::initShaderProgram()
{
    char* vertexShaderSourceCode = NULL; 
    char* fragmentShaderSourceCode = NULL; 
    vertexShaderSourceCode = FileHandler::fileToString("src/shaders/bw.vs"); 
    fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/bw.fs"); 
    if(vertexShaderSourceCode == NULL || fragmentShaderSourceCode == NULL) 
        return false; 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_COLOR, "aColor")); 
	
    shaderProgramObject.create(shaders, attributes); 

    // get uniform locations 
    mvpMatrixUniform = shaderProgramObject.getUniformLocation("uMVPMatrix"); 

    free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
    free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 

    return (true); 
} 

void TestScene::eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{   
    // code 
    bool doImGuiCapturedEvent = false; 
    if(ImGuiManager::initialized == true)
    {
        ImGuiIO& io = ImGui::GetIO(); 
        doImGuiCapturedEvent = io.WantCaptureMouse; 
    } 

    if(!doImGuiCapturedEvent) 
        testSceneCamera.cameraCallback(hwnd, uMsg, wParam, lParam); 

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
