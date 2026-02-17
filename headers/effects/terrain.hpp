#ifndef _TERRAIN_HPP 
#define _TERRAIN_HPP 

#include "../ogldev_array.hpp" 
#include "../fileHandler.hpp" 

#include "../global_header.hpp" 
#include "../common.hpp" 
#include "../shaderProgram.hpp" 

#include <iostream> 
#include <vector> 
#include <string> 

class Terrain; 

class Vertex 
{
    friend class TriangleList; 
    private: 
        vmath::vec3 position; 
        vmath::vec2 texCoord; 
        vmath::vec3 normal; 
        
    public: 
        float getX() const; 
        float getY() const; 
        float getZ() const; 
        void InitVertex(const Terrain* pTerrain, int x, int z); 
}; 

class TriangleList 
{
    private: 
        int m_width = 0; 
        int m_depth = 0; 

        GLuint m_vao; 
        GLuint m_vbo_position; 
        GLuint m_vbo_texCoord; 
        GLuint m_ib; 

    public: 
        TriangleList(); 
        void createTriangleList(int width, int depth, const Terrain* pTerrain); 
        void CreateGLState(); 
        void PopulateBuffers(const Terrain* pTerrain); 
        void InitVertices(const Terrain* pTerrain, std::vector<Vertex>& vertices); 
        void InitIndices(std::vector<unsigned int>& indices); 
        void calculateNormals(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices); 
        void Render(); 
}; 

class Terrain 
{
    private: 
        Array2D<float> m_heightMap; 
        int m_terrainSize = 0; 
        TriangleList m_triangleList; 

        float m_worldScale; 
        float m_heightScale; 
        float m_textureScale; 

        ShaderProgram shaderProgramObject; 
        GLuint mvpMatrixUniform; 
        GLuint textureSamplerUniform[4]; 
        GLuint lightDirectionUniform; 

        GLuint textures[4]; 

    public: 
        Terrain() : m_worldScale(1.0) 
        {
        } 

        
        GLuint loadTerrainTexture(const char* fileName, BOOL bFlipImage) 
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

        bool initialize(float worldScale, float heightScale, int textureScale, std::vector<std::string> terrainTextureFilesPath) 
        {
            m_worldScale = worldScale; 
            m_heightScale = heightScale; 
            m_textureScale = textureScale; 

            // load height map 
            loadFromFile("res/heightmap.save"); 
            m_triangleList.createTriangleList(m_terrainSize, m_terrainSize, this); 
            logFile.log("Terrain heightmap loaded successfully\n");

            initializeShaders(); 

            // check input array of texture images 
            if(terrainTextureFilesPath.size() == 0) 
            {
                logFile.log("Terrain::initialize() > ERROR > provide texture images\n");
                return (false); 
            } 
            else if(terrainTextureFilesPath.size() < 4) 
            {
                // if textures less than 4, then repeat them upto 4 
                for(int i = 0; i < 4 - terrainTextureFilesPath.size(); ++i) 
                    terrainTextureFilesPath.push_back(terrainTextureFilesPath[0]); 
            } 
            
            // load textures 
            for(size_t i = 0; i < 4; ++i) 
            {
                GLuint texture = loadTerrainTexture(terrainTextureFilesPath[i].c_str(), FALSE); 
                textures[i] = texture; 
            }
            logFile.log("Terrain textures loaded successfully\n"); 
    
            return (true); 
        } 

        float GetSize() const 
        {
            return m_terrainSize; 
        } 

        float GetTextureScale() const 
        {
            return m_textureScale; 
        } 

        float GetHeight(int x, int z) const 
        {
            return m_heightMap.Get(x, z); 
        } 

        float GetScale() const 
        {
            return m_worldScale; 
        } 

        float GetHeightScaleFactor() const 
        {
            return m_heightScale; 
        } 

        void Render(mat4 _modelMatrix, mat4 _viewMatrix, mat4 _projectionMatrix) 
        {
            shaderProgramObject.use();
            
            mat4 mvpMatrix = _projectionMatrix * _viewMatrix * _modelMatrix; 
            glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrix); 

            mat4 mvMatrix = _viewMatrix * _modelMatrix; 
            vec3 terrainPosition = vec3(mvMatrix[3][0], mvMatrix[3][1], mvMatrix[3][2]); 
            vec3 lightDirection = -normalize(terrainPosition); 

            // logFile.log("light direction=%.2f,%.2f,%.2f\n", 
            //             lightDirection[0], lightDirection[1], lightDirection[2]); 
            glUniform3fv(lightDirectionUniform, 1, lightDirection); 

            for(size_t i = 0; i < 4; ++i) 
            {
                glActiveTexture(GL_TEXTURE0 + i); 
                glBindTexture(GL_TEXTURE_2D, textures[i]); 
                glUniform1i(textureSamplerUniform[i], i); 
            } 

            m_triangleList.Render(); 

            glBindTexture(GL_TEXTURE_2D, 0); 

            shaderProgramObject.unuse(); 
        } 

        void loadFromFile(const char* pFileName) 
        {
            int FileSize = 0; 
            unsigned char* p = (unsigned char*)FileHandler::readBinaryFile(pFileName, FileSize); 

            assert(FileSize % sizeof(float) == 0); 

            m_terrainSize = sqrt(FileSize / sizeof(float)); 
            m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, p); 
        } 

        void initializeShaders() 
        {
            char* vertexShaderSourceCode = NULL; 
            char* fragmentShaderSourceCode = NULL; 
            vertexShaderSourceCode = FileHandler::fileToString("src/shaders/terrain.vs"); 
            fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/terrain.fs"); 

            std::vector<ShaderSourceCodeAndType> shaders; 
            shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER)); 
            shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER)); 

            std::vector<AttributeWithIndexLocation> attributes; 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_NORMAL, "aNormal")); 
            attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_COLOR, "aColor")); 
            
            shaderProgramObject.create(shaders, attributes); 

            // get uniform locations 
            mvpMatrixUniform = shaderProgramObject.getUniformLocation("uMVPMatrix"); 
            textureSamplerUniform[0] = shaderProgramObject.getUniformLocation("uTextureSampler0"); 
            textureSamplerUniform[1] = shaderProgramObject.getUniformLocation("uTextureSampler1"); 
            textureSamplerUniform[2] = shaderProgramObject.getUniformLocation("uTextureSampler2"); 
            textureSamplerUniform[3] = shaderProgramObject.getUniformLocation("uTextureSampler3"); 
            lightDirectionUniform = shaderProgramObject.getUniformLocation("uLightDirection"); 

            free(vertexShaderSourceCode); vertexShaderSourceCode = NULL; 
            free(fragmentShaderSourceCode); fragmentShaderSourceCode = NULL; 
        } 
}; 

#endif // _TERRAIN_HPP 
