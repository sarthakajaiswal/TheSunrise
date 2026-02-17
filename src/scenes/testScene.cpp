// #include "../../headers/scenes/testScene.hpp" 
// #include "../../headers/shaderProgram.hpp" 
// #include "../../headers/effects/terrain.hpp" 
// #include "../../headers/effects/dissolve.hpp" 

// #define TEST_TERRAIN

// #ifdef TEST_TERRAIN
// static Terrain terrain; 
// #endif // TEST_TERRAIN 

// Dissolve dissolve; 

// Camera testSceneCamera; 
// static float terrainScale = 2.0f; 

// TestScene::TestScene() 
// {
//     // code  
// } 

// int TestScene::initialize() 
// {	
//     std::vector<std::string> terrainTextureImages = 
//     {
//         "res/terrain1.png", 
//         "res/terrain2.png", 
//         "res/terrain3.png", 
//         "res/terrain4.png" 
//     }; 

//     glFrontFace(GL_CW); 

//     #ifdef TEST_TERRAIN
//     int textureScale = 8; 
// 	terrain.initialize(terrainScale, textureScale, terrainTextureImages); 
//     testSceneCamera.initialize(vec3(0.0, 2.0, 5.0), 0.0, 0.0); 
//     #endif // TEST_TERRAIN



//     dissolve.initialize(); 





//     return (0); 
// } 

// float cameraX = 5.0; 
// float cameraY = 300.0; 
// float cameraZ = 10.0; 

// float dissolveValue = 0.0f; 

// void TestScene::display() 
// {
//     mat4 modelMatrix = mat4::identity(); 
//     mat4 viewMatrix = mat4::identity(); 
    
//     #ifdef TEST_TERRAIN
//     viewMatrix = vmath::lookat(vec3(cameraX, cameraY, cameraZ), vec3(125.0*terrainScale, 0.0, 125.0*terrainScale), vec3(0.0, 1.0, 0.0)); //testSceneCamera.getViewMatrix(); 
//     terrain.Render(modelMatrix, viewMatrix, projectionMatrix); 
//     #endif // TEST_TERRAIN

//     // dissolve.render(dissolveValue); 
// } 

// void TestScene::update() 
// {
//     // code 
// } 

// TestScene::~TestScene() 
// {
// } 

// void testSceneCallbacks(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
// {
//     float cameraStep = 0.5f; 
//     switch(uMsg) 
//     {
//         case WM_CHAR: 
//             switch(wParam) 
//             {
//             case 'a': 
//             case 'A': 
//                 cameraX -= cameraStep; 
//                 break; 

//             case 'd': 
//             case 'D': 
//                 cameraX += cameraStep; 
//                 break; 

//             case 'w': 
//             case 'W': 
//                 cameraZ += cameraStep; 
//                 break; 

//             case 's': 
//             case 'S': 
//                 cameraZ -= cameraStep; 
//                 break; 

//             case 'q': 
//             case 'Q': 
//                 cameraY -= cameraStep; 
//                 break; 

//             case 'e': 
//             case 'E': 
//                 cameraY += cameraStep; 
//                 break; 

//             case 'Z': 
//                 dissolveValue += 0.05; 
//                 break; 
//             case 'z': 
//                 dissolveValue -= 0.05; 
//                 break; 

//             default: 
//                 break; 
//             } 
//             break; 

//         default: 
//             break; 
//     } 
// } 

