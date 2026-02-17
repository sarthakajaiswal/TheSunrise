#ifndef _DISSOLVE_HPP 
#define _DISSOLVE_HPP 

#include "../fileHandler.hpp" 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../objects.hpp" 
#include "../shaderProgram.hpp" 

#include <iostream> 
#include <vector> 

class Dissolve 
{
    private: 
        ShaderProgram shaderProgramObject; 
        GLuint mvpMatrixUniform; 
        GLuint dissolveTextureSamplerUniform; 
        GLuint sceneTextureSamplerUniform; 
        GLuint dissolveValueUniform; 

        GLuint dissolveTexture; 
        GLuint texture_scene; 

        Quad quad; 

    public: 
        Dissolve()
        {
        } 
        
        GLuint loadDissolveTexture(const char* fileName, BOOL bFlipImage) 
        {
            // local variable declarations 
            GLuint texture; 
            int width, height, nrComponents; 
            GLuint id; 
            unsigned char* pData = NULL; 

            // code 
            stbi_set_flip_vertically_on_load((bFlipImage == TRUE) ? 1 : 0); 

            pData = stbi_load(fileName, &width, &height, &nrComponents, 0); 
            if(NULL == pData) 
            {
                logFile.log("Load texture failed to load %s\n", fileName); 
                // uninitialize(); 
                exit(EXIT_FAILURE); 
            } 

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
            glGenTextures(1, &id); 
            glBindTexture(GL_TEXTURE_2D, id); 
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

                GLenum format; 
                switch(nrComponents) 
                {
                    case 1: 
                        format = GL_R; 
                        break; 

                    case 2: 
                        format = GL_RG; 
                        break; 

                    case 3: 
                        format = GL_RGB; 
                        break; 

                    case 4: 
                        format = GL_RGBA; 
                        break; 

                    default: 
                        format = GL_RGBA; 
                        break; 
                } 
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pData); 
                glGenerateMipmap(GL_TEXTURE_2D); 
            }
            glBindTexture(GL_TEXTURE_2D, 0); 
            
            stbi_image_free(pData); 
            pData = NULL; 

            stbi_set_flip_vertically_on_load(0); 
            return (id); 
        } 

        void initialize() 
        {
            quad.initialize(); 
            dissolveTexture = loadDissolveTexture("res/dissolve.png", FALSE); 
            initializeShaders(); 

            texture_scene = loadDissolveTexture("res/scene.png", TRUE); 
        } 

        void render(float dissolveValue) 
        {
            shaderProgramObject.use();

            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, dissolveTexture); 
            glUniform1i(dissolveTextureSamplerUniform, 0); 

            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, texture_scene); 
            glUniform1i(sceneTextureSamplerUniform, 1); 

            glUniform1f(dissolveValueUniform, dissolveValue); 

            quad.render(); 

            shaderProgramObject.unuse(); 
        } 

        void initializeShaders() 
        {
            // char* vertexShaderSourceCode = NULL; 
            // char* fragmentShaderSourceCode = NULL; 
            // vertexShaderSourceCode = FileHandler::fileToString("src/shaders/dissolve.vs"); 
            // fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/dissolve.fs"); 
            char* vertexShaderSourceCode = 
            	"#version 460 core\n" \
                "in vec4 aPosition;\n" \
                "in vec2 aTexCoord;\n" \
                "out vec2 out_TexCoord;\n" \
                "void main(void) \n" \
                "{\n" \
                "gl_Position = aPosition;\n" \
                "out_TexCoord = aTexCoord;\n" \
                "}\n"; 

            char* fragmentShaderSourceCode = 
                "#version 460 core\n" \
                "in vec2 out_TexCoord;\n" \
                
                "uniform sampler2D uDissolveTextureSampler;\n" \
                "uniform sampler2D uSceneTextureSampler;\n" \
                "uniform float uDissolveValue;\n" \

                "out vec4 FragColor;\n" \
                
                "void main(void)\n" \
                "{\n" \
                "   vec3 originalColor = texture(uSceneTextureSampler, out_TexCoord).rgb;\n" \
                "   vec4 finalColor = vec4(originalColor, 1.0);\n" \
                
                // get value of red component of texture, 
                "   float dissolveFactor = texture(uDissolveTextureSampler, out_TexCoord).r;\n" \
                "   dissolveFactor = dissolveFactor - uDissolveValue;\n" \
                
                // if above value (r component) is below specific value we do not render that pixel 
                "   if(dissolveFactor < 0.1)\n" \
                "   {\n" \
                "       discard;\n" \
                "       return;\n" \
                "   }\n" \

                // blending the area near black pixels 
                "   else if(dissolveFactor < 0.3)\n" \
                "   {\n" \
                "       float isGlowing = smoothstep(0.05, 0.3, dissolveFactor);\n" \
                "       vec3 glowColor = vec3(1.0, 0.0, 0.0) * (1.0-isGlowing);\n" \
                "       finalColor += vec4(glowColor, 1.0);\n" \
                "   }\n" \

                "   FragColor = finalColor;\n" \
                "}\n"; 	

            std::vector<ShaderSourceCodeAndType> shaders; 
            shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
            shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

            std::vector<AttributeWithIndexLocation> attributes; 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_COLOR, "aColor")); 
            
            shaderProgramObject.create(shaders, attributes); 

            // get uniform locations 
            mvpMatrixUniform = shaderProgramObject.getUniformLocation("uMVPMatrix"); 
            dissolveTextureSamplerUniform = shaderProgramObject.getUniformLocation("uDissolveTextureSampler"); 
            dissolveValueUniform = shaderProgramObject.getUniformLocation("uDissolveValue"); 
            sceneTextureSamplerUniform = shaderProgramObject.getUniformLocation("uSceneTextureSampler"); 

            // free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
            // free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
        } 
}; 

#endif // _DISSOLVE_HPP 
