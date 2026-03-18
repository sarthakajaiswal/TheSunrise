#include "..\..\headers\effects\terrain.hpp" 

heightmap_loading_failure::heightmap_loading_failure(const char* msg) : std::runtime_error(msg) 
{
} 

opengl_initialization_failure::opengl_initialization_failure(const char* msg) : std::runtime_error(msg) 
{
} 

render_called_before_initialize::render_called_before_initialize(const char* msg) : std::runtime_error(msg) 
{
} 

no_texture_images_provided::no_texture_images_provided(const char* msg) : std::runtime_error(msg) 
{
} 

void Terrain::showHeightMapData() 
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

void Terrain::showVertices() 
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

void Terrain::InitVertices(int width, int depth, float* heightData, std::vector<Vertex>& vertices) 
{
    logFile.log("textureScale = %.2f\n", textureScale); 
    for(int z = 0; z < depth; ++z) 
    {
        for(int x = 0; x < width; ++x) 
        {
            Vertex v; 

            v.position[0] = x * worldScale; 
            v.position[1] = heightData[z * width + x] * heightScale; 
            v.position[2] = z * worldScale; 

            // calculate texcoords 
            v.texCoord[0] = (float)x/(float)(width-1) * textureScale; 
            v.texCoord[1] = (float)z/(float)(depth-1) * textureScale; 

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
            normal = vmath::vec3((hLeft-hRight)*heightScale, 2.0, (hUp-hDown)*heightScale); // 2.0 gives better terrain | mathematically calculated using cross product 

            v.normal = normalize(normal); 

            vertices.push_back(v); 
        } 
    } 
} 

void Terrain::InitIndices(int width, int depth, std::vector<unsigned int>& indices) 
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

float* Terrain::LoadHeightMap(const char* filePath, int* pWidth, int* pDepth) 
{
    // variable declarations 
    unsigned char* pRawData; 
    int width, depth, nrComponents; 
    float* pHeightData; 

    pRawData = stbi_load(filePath, &width, &depth, &nrComponents, 1); 
    if(NULL == pRawData) 
    {
        char errorStr[255]; 
        sprintf(errorStr, "Failed to load texture : %s\n", filePath); 
        throw texture_loading_failure(errorStr); 
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

int Terrain::InitOpenGLState() 
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

    createTerrainProgram(); 
    createOcclusionProgram(); 

    return (0); 
} 

void Terrain::createTerrainProgram() 
{
    /*************** terrain program ****************/
    char* vertexShaderSourceCode = FileHandler::fileToString("src/shaders/terrain.vs"); 
    char* fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/terrain.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER));  
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER));  

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_TEXCOORD, "aTexCoord")); 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_NORMAL, "aNormal")); 

    terrainShaderProgram.create(shaders, attributes); 

    if(vertexShaderSourceCode)
    {
        free(vertexShaderSourceCode); 
        vertexShaderSourceCode = 0; 
    } 
    if(fragmentShaderSourceCode) 
    {
        free(fragmentShaderSourceCode); 
        fragmentShaderSourceCode = 0; 
    } 

    // get the required uniform location from shader 
    modelMatrixUniform          = terrainShaderProgram.getUniformLocation("uModelMatrix"); 
    viewMatrixUniform           = terrainShaderProgram.getUniformLocation("uViewMatrix"); 
    projectionMatrixUniform     = terrainShaderProgram.getUniformLocation("uProjectionMatrix"); 
    normalMatrixUniform         = terrainShaderProgram.getUniformLocation("uNormalMatrix"); 
    
    heightScaleUniform          = terrainShaderProgram.getUniformLocation("uHeightScale"); 
    texture1Uniform             = terrainShaderProgram.getUniformLocation("uTextures[0]"); 
    texture2Uniform             = terrainShaderProgram.getUniformLocation("uTextures[1]"); 
    texture3Uniform             = terrainShaderProgram.getUniformLocation("uTextures[2]"); 
    texture4Uniform             = terrainShaderProgram.getUniformLocation("uTextures[3]"); 
    
    hRange1Uniform              = terrainShaderProgram.getUniformLocation("hRange1"); 
    hRange2Uniform              = terrainShaderProgram.getUniformLocation("hRange2"); 
    hRange3Uniform              = terrainShaderProgram.getUniformLocation("hRange3"); 
    hRange4Uniform              = terrainShaderProgram.getUniformLocation("hRange4"); 
    
    viewPositionUniform         = terrainShaderProgram.getUniformLocation("uViewPosition"); 
    
    isLightUniform              = terrainShaderProgram.getUniformLocation("uIsLightEnabled"); 
    lightColorUniform           = terrainShaderProgram.getUniformLocation("uLightColor"); 
    lightPositionUniform        = terrainShaderProgram.getUniformLocation("uLightPosition"); 
    isFogUniform                = terrainShaderProgram.getUniformLocation("uIsFogEnabled"); 

    isFogUniform                = terrainShaderProgram.getUniformLocation("uIsFogEnabled"); 
    fogStartUniform             = terrainShaderProgram.getUniformLocation("uFogStart"); 
    fogEndUniform               = terrainShaderProgram.getUniformLocation("uFogEnd"); 
    fogColorUniform             = terrainShaderProgram.getUniformLocation("uFogColor"); 
} 

void Terrain::createOcclusionProgram() 
{
    /*************** occlusion program ****************/ 
    char* vertexShaderSourceCode = FileHandler::fileToString("src/shaders/occlusion.vs"); 
    char* fragmentShaderSourceCode = FileHandler::fileToString("src/shaders/occlusion.fs"); 

    std::vector<ShaderSourceCodeAndType> shaders; 
    shaders.push_back(ShaderSourceCodeAndType(vertexShaderSourceCode, GL_VERTEX_SHADER));  
    shaders.push_back(ShaderSourceCodeAndType(fragmentShaderSourceCode, GL_FRAGMENT_SHADER));  

    std::vector<AttributeWithIndexLocation> attributes; 
    attributes.push_back(AttributeWithIndexLocation(AMC_ATTRIBUTE_POSITION, "aPosition")); 

    occlusionProgram.create(shaders, attributes); 

    if(vertexShaderSourceCode)
    {
        free(vertexShaderSourceCode); 
        vertexShaderSourceCode = 0; 
    } 
    if(fragmentShaderSourceCode) 
    {
        free(fragmentShaderSourceCode); 
        fragmentShaderSourceCode = 0; 
    } 

    mvpMatrixUniform_occlusion = occlusionProgram.getUniformLocation("uMVPMatrix"); 
}

Terrain::Terrain() 
{
    vao = 0; 
    vbo = 0; 
    ebo = 0; 
    isInitialized = false; 
}  

int Terrain::initialize(const char* heightmapImagePath, float _worldScale, float _heightScale, 
                std::vector<std::string> textureImages, std::vector<float> _textureHeightRanges, float _textureScale) 
{
    // code 
    logFile.log("---- Terrain::initialize() started ----\n"); 
    
    heightmapData = LoadHeightMap(heightmapImagePath, &this->width, &this->depth); 
    if(heightmapData == 0) 
        throw heightmap_loading_failure("Terrain::initialize() > heightmap loading failed\n"); 
    else 
        logFile.log("heightmap image for terrain loaded successfully\n"); 

    assert(this->width != 0); 
    assert(this->depth != 0);   

    this->worldScale = _worldScale; 
    this->heightScale = _heightScale; 
    this->textureScale = _textureScale; 
    InitVertices(width, depth, heightmapData, mesh); 
    // showHeightMapData(); 
    // showVertices(); 
    InitIndices(width, depth, indices); 
    assert(InitOpenGLState() == 0); 

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

    logFile.log("---- Terrain::initialize() completed successfully ----\n"); 
    return (0); 
} 

void Terrain::render(
    vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix, 
    vec3 viewPosition, 
    bool bLight, bool bFog, 
    vec3 lightPosition, vec3 lightColor, 
    float fogStart, float fogEnd, vec3 fogColor, 
    vec4 clipPlane 
) 
{
    terrainShaderProgram.use(); 

    glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, _modelMatrix); 
    glUniformMatrix4fv(viewMatrixUniform, 1, GL_FALSE, _viewMatrix); 
    glUniformMatrix4fv(projectionMatrixUniform, 1, GL_FALSE, _projectionMatrix);  
    // glUniformMatrix4fv(normalMatrixUniform, 1, GL_FALSE, (_modelMatrix).transpose()); 

    glUniform1f(heightScaleUniform, heightScale); 
    glUniform1f(hRange1Uniform, textureHeightRanges[0]); 
    glUniform1f(hRange2Uniform, textureHeightRanges[1]); 
    glUniform1f(hRange3Uniform, textureHeightRanges[2]); 
    glUniform1f(hRange4Uniform, textureHeightRanges[3]); 

    glUniform3fv(viewPositionUniform, 1, viewPosition); 

    if(bLight == true) 
    {
        glUniform1i(isLightUniform, 1); 
        glUniform3fv(lightPositionUniform, 1, lightPosition); 
        glUniform3fv(lightColorUniform, 1, lightColor); 
    } 
    else 
    {
        glUniform1i(isLightUniform, 0); 
    } 

    if(bFog == true) 
    {
        glUniform1i(isFogUniform, 1); 
        glUniform1f(fogStartUniform, fogStart); 
        glUniform1f(fogEndUniform, fogEnd); 
        glUniform3fv(fogColorUniform, 1, fogColor); 
    } 
    else 
    {
        glUniform1i(isFogUniform, 0); 
    } 

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

    float maxAniso = 0.0f; 
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, maxAniso); 

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0); 

    terrainShaderProgram.unuse(); 
} 

void Terrain::renderOcclusion(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix) 
{
    occlusionProgram.use(); 
    glUniformMatrix4fv(mvpMatrixUniform_occlusion, 1, GL_FALSE, _projectionMatrix * _viewMatrix * _modelMatrix); 
    
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0); 

    occlusionProgram.unuse(); 
} 

void Terrain::uninitialize() 
{
    // code 
    logFile.log("Terrain::uninitialize() >  uninitializing terrain...\n"); 
    
    for(int i = 0; i < textures.size(); ++i) 
    {
        glDeleteTextures(1, &textures[i]); 
        textures[i] = 0; 
    } 

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

    logFile.log("Terrain::uninitialize() >  uninitialized...\n"); 
} 

Terrain::~Terrain() 
{
    delete[] heightmapData; 
    heightmapData = 0;
} 
