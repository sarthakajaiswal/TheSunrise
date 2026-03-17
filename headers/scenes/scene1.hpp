#pragma once 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 
#include "../effects/exposure.hpp" 
#include "../effects/terrain.hpp" 
#include "../effects/cubemap.hpp" 
#include "../effects/godrays.hpp" 
#include "../effects/textureBlending.hpp" 
#include "../objects.hpp"
#include "../assimpModelLoader.hpp"
#include "../fbo.hpp" 
#include "../../ImGui/imgui.h" 
#include "../imgui_manager.hpp" 

class Scene1 
{
    private:
        Exposure exposureProgram; 
        FullScreenTexturer fsTexturer;  
        Terrain terrain; 
        Godrays godrays; 
        Cubemap cubemap; 
        TextureBlending textureBlender; 

        ShaderProgram bwShader; 
        GLuint mvpMatrixUniform_bwShader; 
        
        Model treeModel; 
        Model mindFlare; 
        Quad quad; 
        Sphere moonSphere; 

        FBO floatingPointFBO; 
        FBO sceneFBO; 

        GLuint quoteTexture; 
        GLuint gateTexture; 

        int initBWShader(); 

    public: 
        float exposure_godrays; 
        float decay_godrays; 
        float density_godrays; 
        float weight_godrays; 
        float strength_godrays; 
        int numSamples_godrays; 

        Scene1(); 
        int initialize(); 
        void display(); 
        void update(); 
        void uninitialize(); 
        ~Scene1(); 

        void eventCallback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);  
}; 


