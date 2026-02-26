#pragma once 

#include <iostream> 
#include <string> 
#include <vector> 
#include <cmath> 
#include <stdexcept> 
#include <cassert> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "..\vmath.h" 
#include "..\stb_image.h" 
#include "..\fileHandler.hpp" 
#include "..\global_header.hpp" 
#include "..\camera.hpp" 

extern FileHandler logFile; 
extern Camera testSceneCamera; 

struct Vertex 
{
    vmath::vec3 position; 
    vmath::vec2 texCoord; 
    vmath::vec3 normal; 
}; 

class heightmap_loading_failure : public std::runtime_error 
{
    public: 
        heightmap_loading_failure(const char* msg) : std::runtime_error(msg) 
        {
        } 
}; 

class opengl_initialization_failure : public std::runtime_error 
{
    public: 
        opengl_initialization_failure(const char* msg) : std::runtime_error(msg) 
        {
        } 
}; 

class render_called_before_initialize : public std::runtime_error 
{
    public: 
        render_called_before_initialize(const char* msg) : std::runtime_error(msg) 
        {
        } 
}; 

class no_texture_images_provided : public std::runtime_error 
{
    public: 
        no_texture_images_provided(const char* msg) : std::runtime_error(msg) 
        {
        } 
}; 

class Terrain 
{
    private: 
        std::vector<Vertex> mesh; 
        float* heightmapData; 
        int width, depth; 
        float worldScale; 
        float heightScale; 
        float textureScale; 

        std::vector<GLuint> textures; 
        std::vector<float> textureHeightRanges; 

        GLuint shaderProgramObject; 
        GLuint modelMatrixUniform; 
        GLuint viewMatrixUniform; 
        GLuint projectionMatrixUniform; 
        GLuint normalMatrixUniform; 
        GLuint heightScaleUniform; 
        GLuint texture1Uniform; 
        GLuint texture2Uniform; 
        GLuint texture3Uniform; 
        GLuint texture4Uniform; 
        GLuint hRange1Uniform; 
        GLuint hRange2Uniform; 
        GLuint hRange3Uniform; 
        GLuint hRange4Uniform; 
        GLuint viewPositionUniform; 


        std::vector<unsigned int> indices; 
        GLuint vao, vbo, ebo; 

        bool isInitialized; 

        void showHeightMapData() 
        {
            for(unsigned int i = 0; i < depth; ++i) 
            {
                for(unsigned int j = 0; j < width; ++j) 
                {
                    logFile.log("%.3f   ", heightmapData[i*width+j]); 
                } 
                logFile.log("\n"); 
            } 
        } 

        void showVertices() 
        {
            for(unsigned int i = 0; i < depth; ++i) 
            {
                for(unsigned int j = 0; j < width; ++j) 
                {
                    logFile.log("%.2f,%.2f,%.2f   ", 
                            mesh[i*width+j].position[0], 
                            mesh[i*width+j].position[1], 
                            mesh[i*width+j].position[2]  
                        ); 
                } 
                logFile.log("\n"); 
            } 
        } 

        void InitVertices(int width, int depth, float* heightData, std::vector<Vertex>& vertices) 
        {
            for(int z = 0; z < depth; ++z) 
            {
                for(int x = 0; x < width; ++x) 
                {
                    Vertex v; 

                    v.position[0] = x * worldScale; 
                    v.position[1] = heightData[z * width + x] * heightScale; 
                    v.position[2] = z * worldScale; 

                    // calculate texcoords 
                    v.texCoord[0] = 1.0/(float)(width-1) * x * textureScale; 
                    v.texCoord[1] = 1.0/(float)(depth-1) * z * textureScale; 

                    // calculate normals 
                    vmath::vec3 normal; 
                    // 1) clamp the indices to avoid accessing out of bound indices (like -1, width, depth) 
                    int leftX = max(0, x-1); 
                    int rightX = min(x+1, width-1); 
                    int upZ = max(0, z-1); 
                    int downZ = min(z+1, depth-1); 

                    // 2) get neighbours height and clamped indices 
                    float hLeft = heightData[z * width + leftX];   
                    float hRight = heightData[z * width + rightX];  
                    float hUp = heightData[upZ * width + x];     
                    float hDown = heightData[downZ * width + x];   

                    // use height of neighbours to calculate normal 
                    normal = vmath::vec3(hLeft-hRight, 2.0, hUp-hDown); // 2.0 gives better terrain | mathematically calculated using cross product 

                    v.normal = normalize(normal); 

                    vertices.push_back(v); 
                } 
            } 
        } 

        void InitIndices(int width, int depth, std::vector<unsigned int>& indices) 
        {
            for(int z = 0; z < depth-1; ++z) 
            {
                for(int x = 0; x < width-1; ++x) 
                { 
                    unsigned int TR = z * width + (x+1);    // top right 
                    unsigned int TL = z * width + x;        // top left 
                    unsigned int BL = (z+1) * width + x;    // bottom left 
                    unsigned int BR = (z+1) * width + (x+1); // bottom right 

                    indices.push_back(TR); 
                    indices.push_back(TL); 
                    indices.push_back(BL);
                    
                    indices.push_back(TR); 
                    indices.push_back(BL); 
                    indices.push_back(BR); 
                } 
            } 
        } 

        float* LoadHeightMap(const char* filePath, int* pWidth, int* pDepth) 
        {
            // variable declarations 
            unsigned char* pRawData; 
            int width, depth, nrComponents; 
            float* pHeightData; 

            pRawData = stbi_load(filePath, &width, &depth, &nrComponents, 1); 
            if(NULL == pRawData) 
            {
                logFile.log("Terrain()::LoadHeightmap() > failed to load heightmap %s\n", filePath); 
                // TODO: throw exception here 
                exit(EXIT_FAILURE); 
            } 

            pHeightData = new float[width * depth]; 

            for(int z = 0; z < depth; ++z) 
            {
                for(int x = 0; x < width; ++x) 
                {
                    int index = z * width + x; 
                    float heightAtIndex = ((float)(pRawData[index])/255.0); 
                    pHeightData[index] = heightAtIndex; 
                }
            } 

            *pWidth = width; 
            *pDepth = depth; 

            stbi_image_free(pRawData); 
            pRawData = 0; 

            return (pHeightData); 
        }  

        int InitOpenGLState() 
        {
            // function declarations 
            void uninitialize(void); 

            // code 
            // fill buffers 
            glCreateVertexArrays(1, &vao); 
            glBindVertexArray(vao); 

            glCreateBuffers(1, &vbo); 
            glBindBuffer(GL_ARRAY_BUFFER, vbo); 
            glBufferData(GL_ARRAY_BUFFER, mesh.size()*sizeof(Vertex), mesh.data(), GL_STATIC_DRAW); 
            int stride = sizeof(Vertex); 
            glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0*sizeof(float))); 
            glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
			glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float))); 
			glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);  
            glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5*sizeof(float))); 
			glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL); 
            glBindBuffer(GL_ARRAY_BUFFER, 0); 

            glCreateBuffers(1, &ebo); 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); 
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); 

            glBindVertexArray(0); 

            // init shader program 
            const GLchar* vertexShaderSourceCode = 
            "#version 460 core\n" \

            "in vec4 aPosition;\n" \
            "in vec2 aTexCoord;\n" \
            "in vec3 aNormal;\n" \

            "out vec2 out_texCoord;\n" \
            "out vec3 out_normal;\n" \
            "out vec3 out_worldPosition;\n" \
            
            "uniform mat4 uModelMatrix;\n" \
            "uniform mat4 uViewMatrix;\n" \
            "uniform mat4 uProjectionMatrix;\n" \
            "uniform mat4 uNormalMatrix;\n" \
            
            "void main(void) \n" \
            "{\n" \
            "   gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * aPosition;\n" \
            "   out_worldPosition = (uModelMatrix * aPosition).xyz;\n" \
            "   out_texCoord = aTexCoord;\n" \
            "   out_normal = (mat3(transpose(inverse(uModelMatrix))) * aNormal).xyz;\n" \
            "   out_normal = normalize(out_normal);\n" \
            "}\n"; 

            GLuint vertexShaderObject = glCreateShader(GL_VERTEX_SHADER); 
            glShaderSource(vertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL); 
            glCompileShader(vertexShaderObject); 

            GLint status = 0; 
            GLint infoLogLength = 0; 
            GLchar* szInfoLog = NULL; 
            glGetShaderiv(vertexShaderObject, GL_COMPILE_STATUS, &status); 
            if(status == GL_FALSE) 
            {
                glGetShaderiv(vertexShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength); 
                if(infoLogLength > 0) 
                {
                    szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar)); 
                    if(szInfoLog != NULL) 
                    {
                        glGetShaderInfoLog(vertexShaderObject, infoLogLength, NULL, szInfoLog); 
                        logFile.log("Terrain::vertex shader compilation log = %s\n", szInfoLog);  
                    
                        free(szInfoLog); 
                        szInfoLog = NULL; 
                    }
                }

                uninitialize(); 
            } 
            
            // ======================= FRAGMENT SHADER =========================== 
            const char* fragmentShaderSourceCode = 
            "#version 460 core\n" \

            "in vec2 out_texCoord;\n" \
            "in vec3 out_normal;\n" \
            "in vec3 out_worldPosition;\n" \

            "out vec4 FragColor;\n" \

            "uniform sampler2D uTextures[4];\n" \
            "uniform float uHeightScale;\n" \

            // "const float hRange1 = 0.1;\n" \
            // "const float hRange2 = 0.3;\n" \
            // "const float hRange3 = 0.5;\n" \
            // "const float hRange4 = 0.75;\n" \
            
            "uniform float hRange1;\n" \
            "uniform float hRange2;\n" \
            "uniform float hRange3;\n" \
            "uniform float hRange4;\n" \
            
            "vec4 uLightPosition = vec4(0.0, 50.0, 50.0, 1.0);\n" \
            "vec3 lightColor = vec3(1.0, 1.0, 1.0);\n" \
            "uniform vec3 uViewPosition;\n" \

            "void main(void)\n" \
            "{\n" \
            /* -------- light color --------- */
            "   vec3 skyColor = vec3(0.7, 0.9, 0.9);\n" \
            "   vec3 groundColor = vec3(0.4, 0.3, 0.2);\n" \
            "   float mixFactor = dot(out_normal, vec3(0.0, 1.0, 0.0)) * 0.5 + 0.5;\n" \
            "   vec3 ambientColor = mix(groundColor, skyColor, mixFactor);\n" \

            "   vec3 lightDirection = normalize(vec3(out_worldPosition - uLightPosition.xyz));\n" \
            "   float diffuse = max(0.0, dot(-lightDirection, out_normal));\n" \
            "   vec3 diffuseColor = diffuse * lightColor;\n" \

            "   float shininess = 32.0f;\n" \
            "   vec3 reflectionVector = normalize(reflect(lightDirection, out_normal));\n" \
            "   vec3 viewDirection = normalize(vec3(uViewPosition - out_worldPosition));\n" \
            "   float specular = pow(max(0.0, dot(reflectionVector, viewDirection)), shininess);\n" \
            "   vec3 specularColor = specular * lightColor;\n" \

            "   lightColor = (ambientColor + diffuseColor + specularColor);\n" \

            /* -------- texture color --------- */ 
            "   float height = out_worldPosition.y/uHeightScale;\n" \
            
            "   vec3 textureColor = vec3(0.0);\n" \
            "   if(height < hRange1)\n" \
            "   {\n" \
            "       textureColor = texture(uTextures[0], out_texCoord).rgb;\n" \
            "   }\n" \
            "   else if(height < hRange2)\n" \
            "   {\n" \
            "       vec3 color0 = texture(uTextures[0], out_texCoord).rgb;\n" \
            "       vec3 color1 = texture(uTextures[1], out_texCoord).rgb;\n" \
            "       float delta = hRange2-hRange1;\n" \
            "       float factor = (height - hRange1)/delta;\n" \
            "       textureColor = mix(color0, color1, factor);\n" \
            "   }\n" \
            "   else if(height < hRange3)\n" \
            "   {\n" \
            "       vec3 color0 = texture(uTextures[1], out_texCoord).rgb;\n" \
            "       vec3 color1 = texture(uTextures[2], out_texCoord).rgb;\n" \
            "       float delta = hRange3-hRange2;\n" \
            "       float factor = (height - hRange2)/delta;\n" \
            "       textureColor = mix(color0, color1, factor);\n" \
            "   }\n" \
            "   else if(height < hRange4)\n" \
            "   {\n" \
            "       vec3 color0 = texture(uTextures[2], out_texCoord).rgb;\n" \
            "       vec3 color1 = texture(uTextures[3], out_texCoord).rgb;\n" \
            "       float delta = hRange4-hRange3;\n" \
            "       float factor = (height - hRange3)/delta;\n" \
            "       textureColor = mix(color0, color1, factor);\n" \
            "   }\n" \
            "   else\n" \
            "   {\n" \
            "       textureColor = texture(uTextures[3], out_texCoord).rgb;\n" \
            "   }\n" \

            /* ----- final color ----- */ 
            "   FragColor = vec4(textureColor * lightColor, 1.0f);\n" \
            "}\n"; 

            GLuint fragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER); 

            glShaderSource(fragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL); 
            glCompileShader(fragmentShaderObject); 

            status = 0; 
            infoLogLength = 0; 
            szInfoLog = NULL; 

            glGetShaderiv(fragmentShaderObject, GL_COMPILE_STATUS, &status); 
            if(status == GL_FALSE) 
            {
                glGetShaderiv(fragmentShaderObject, GL_INFO_LOG_LENGTH, &infoLogLength); 
                if(infoLogLength > 0) 
                {
                    szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar)); 
                    if(szInfoLog != NULL) 
                    {
                        glGetShaderInfoLog(fragmentShaderObject, infoLogLength, NULL, szInfoLog); 
                        logFile.log("Terrain::Fragment shader compilation log = %s\n", szInfoLog); 

                        free(szInfoLog); 
                        szInfoLog = NULL; 
                    }
                }

                uninitialize(); 
            }

            shaderProgramObject = glCreateProgram(); 
            glAttachShader(shaderProgramObject, vertexShaderObject); 
            glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition"); 
            glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord"); 
            glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal"); 
            glAttachShader(shaderProgramObject, fragmentShaderObject); 

            glLinkProgram(shaderProgramObject); 
            status = 0; 
            infoLogLength = 0; 
            szInfoLog = 0; 
            glGetProgramiv(shaderProgramObject, GL_LINK_STATUS, &status); 
            if(status == GL_FALSE) 
            {
                glGetProgramiv(shaderProgramObject, GL_INFO_LOG_LENGTH, &infoLogLength); 
                if(infoLogLength > 0) 
                {
                    szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar)); 
                    if(szInfoLog != NULL) 
                    {
                        glGetProgramInfoLog(shaderProgramObject, infoLogLength, NULL, szInfoLog); 
                        logFile.log("Terrain::shader program object = %s\n", szInfoLog); 

                        free(szInfoLog); 
                        szInfoLog = NULL; 
                    }
                }

                uninitialize(); 
            } 

            // get the required uniform location from shader 
            modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix"); 
            viewMatrixUniform = glGetUniformLocation(shaderProgramObject, "uViewMatrix"); 
            projectionMatrixUniform = glGetUniformLocation(shaderProgramObject, "uProjectionMatrix"); 
            normalMatrixUniform = glGetUniformLocation(shaderProgramObject, "uNormalMatrix"); 
            heightScaleUniform = glGetUniformLocation(shaderProgramObject, "uHeightScale"); 
            texture1Uniform = glGetUniformLocation(shaderProgramObject, "uTextures[0]"); 
            texture2Uniform = glGetUniformLocation(shaderProgramObject, "uTextures[1]"); 
            texture3Uniform = glGetUniformLocation(shaderProgramObject, "uTextures[2]"); 
            texture4Uniform = glGetUniformLocation(shaderProgramObject, "uTextures[3]"); 
            hRange1Uniform = glGetUniformLocation(shaderProgramObject, "hRange1"); 
            hRange2Uniform = glGetUniformLocation(shaderProgramObject, "hRange2"); 
            hRange3Uniform = glGetUniformLocation(shaderProgramObject, "hRange3"); 
            hRange4Uniform = glGetUniformLocation(shaderProgramObject, "hRange4"); 
            viewPositionUniform = glGetUniformLocation(shaderProgramObject, "uViewPosition"); 

            return (0); 
        } 

        int loadTexture(const char* path, BOOL bFlipImage = TRUE) 
        {
            // local variable declarations 
            GLuint texture; 
            unsigned char* pData = NULL; 
            int width, height, nrComponents; 

            // code 
            stbi_set_flip_vertically_on_load((bFlipImage == TRUE) ? 1 : 0); 

            pData = stbi_load(path, &width, &height, &nrComponents, 0); 
            if(NULL == pData) 
            {
                logFile.log("Load texture failed to load %s\n", path); 
                // uninitialize(); 
                exit(EXIT_FAILURE); 
            } 

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
            glGenTextures(1, &texture); 
            glBindTexture(GL_TEXTURE_2D, texture); 
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
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
            return (texture); 
        } 

    public: 
        Terrain() 
        {
            shaderProgramObject = 0; 
            vao = 0; 
            vbo = 0; 
            ebo = 0; 
            isInitialized = false; 
        }  

        int initialize(const char* heightmapImagePath, float _worldScale, float _heightScale, 
                        std::vector<std::string> textureImages, std::vector<float> _textureHeightRanges, float _textureScale) 
        {
            // code 
            heightmapData = LoadHeightMap(heightmapImagePath, &this->width, &this->depth); 
            if(heightmapData == 0) 
                throw heightmap_loading_failure("Terrain::initialize() > heightmap loading failed\n"); 
            assert(this->width != 0); 
            assert(this->depth != 0);   

            this->worldScale = _worldScale; 
            this->heightScale = _heightScale; 
            this->textureScale = _textureScale; 
            InitVertices(width, depth, heightmapData, mesh); 

            // showHeightMapData(); 
            // showVertices(); 

            InitIndices(width, depth, indices); 
            if(InitOpenGLState() != 0) 
                throw opengl_initialization_failure("Terrain::initialize() > InitOpenGLState() failed\n"); 

            // textures 
            if(textureImages.size() < 4) 
                throw no_texture_images_provided("Terrain::initialize() > Provide 4 texture images\n"); 
            for(unsigned int i = 0; i < clamp((int)textureImages.size(), 0, 4); ++i)    // clamp() exnsures more than 4 textures are not loaded 
                textures.push_back(loadTexture(textureImages[i].c_str())); 
            
            // texture height ranges 
            if(textureImages.size() < 4) 
            {
                this->textureHeightRanges.push_back(0.1); 
                this->textureHeightRanges.push_back(0.3); 
                this->textureHeightRanges.push_back(0.5); 
                this->textureHeightRanges.push_back(0.75); 
            }
            else 
            {
                for(unsigned int i = 0; i < clamp((int)textureImages.size(), 0, 4); ++i) 
                    this->textureHeightRanges.push_back(_textureHeightRanges[i]);  
            } 

            isInitialized = true; 
            return (0); 
        } 

        void render(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix) 
        {
            if(isInitialized == false) 
                throw render_called_before_initialize("Terrain::render() > terrain is not initialized yet\n"); 

            glUseProgram(shaderProgramObject); 

            glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, _modelMatrix); 
            glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, _viewMatrix); 
            glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, _projectionMatrix);  
            // glUniformMatrix4fv(normalMatrixUniform, 1, GL_FALSE, (_modelMatrix).transpose()); 

            glUniform1f(heightScaleUniform, heightScale); 
            glUniform1f(hRange1Uniform, textureHeightRanges[0]); 
            glUniform1f(hRange2Uniform, textureHeightRanges[1]); 
            glUniform1f(hRange3Uniform, textureHeightRanges[2]); 
            glUniform1f(hRange4Uniform, textureHeightRanges[3]); 

            glUniform3fv(viewPositionUniform, 1, testSceneCamera.getPosition()); 

            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, textures[0]); 
            glUniform1i(texture1Uniform, 0); 

            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, textures[1]); 
            glUniform1i(texture2Uniform, 1); 

            glActiveTexture(GL_TEXTURE2); 
            glBindTexture(GL_TEXTURE_2D, textures[2]); 
            glUniform1i(texture3Uniform, 2); 

            glActiveTexture(GL_TEXTURE3); 
            glBindTexture(GL_TEXTURE_2D, textures[3]); 
            glUniform1i(texture4Uniform, 3); 
 
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
            glBindVertexArray(0); 

            glUseProgram(0); 
        } 

        void uninitialize() 
        {
            if(ebo) 
            {
                glDeleteBuffers(1, &ebo); 
                ebo = 0; 
            } 
            if(vbo) 
            {
                glDeleteBuffers(1, &vbo); 
                vbo = 0; 
            } 
            if(vao) 
            {
                glDeleteVertexArrays(1, &vao); 
                vao = 0; 
            } 

            if(shaderProgramObject) 
            {
                glUseProgram(shaderProgramObject); 
                
                GLint numShaders; 
                glGetProgramiv(shaderProgramObject, GL_ATTACHED_SHADERS, &numShaders); 
                if(numShaders > 0) 
                {
                    GLuint* pShaders = (GLuint*)malloc(numShaders * sizeof(GLuint)); 
                    if(pShaders != NULL) 
                    {
                        glGetAttachedShaders(shaderProgramObject, numShaders, NULL, pShaders);
                        
                        for(GLint i = 0; i < numShaders; ++i) 
                        {
                            glDetachShader(shaderProgramObject, pShaders[i]); 
                            glDeleteShader(pShaders[i]); 
                            pShaders[i] = 0; 
                        }

                        free(pShaders); 
                        pShaders = 0; 
                    }
                }
            } 
            glUseProgram(0); 
        } 

        ~Terrain() 
        {
            delete[] heightmapData; 
            heightmapData = 0;
        } 
}; 

