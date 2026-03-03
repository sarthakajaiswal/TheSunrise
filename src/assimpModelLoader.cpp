#include "../headers/assimpModelLoader.hpp" 

Model::Mesh::Mesh() 
{
    nodeTransform = mat4::identity(); 
    vao = 0; 
    vbo = 0; 
    ebo = 0; 
} 

std::vector<ModelTexture> 
Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) 
{
    std::vector<ModelTexture> textures; 

    for(unsigned int i = 0; i < mat->GetTextureCount(type); ++i) 
    {
        aiString str; 
        mat->GetTexture(type, i, &str); 
        std::string fullPath = directory + "/" + str.C_Str(); 

        // skip if already loaded 
        bool alreadyLoaded = false; 
        for(unsigned int j = 0; j < loadedTextures.size(); ++j) 
        {
            if(loadedTextures[j].path == str.C_Str())
            {
                textures.push_back(loadedTextures[j]); 
                alreadyLoaded = true; 
                break; 
            } 
        } 
        if(!alreadyLoaded)  
        { 
            ModelTexture texture; 
            texture.id = loadTextureFromFile(fullPath.c_str()); 
            texture.type = typeName; 
            texture.path = str.C_Str(); 
            textures.push_back(texture); 
            loadedTextures.push_back(texture); 
        } 
    } 

    return (textures); 
} 

GLuint 
Model::loadTextureFromFile(const char* filePath) 
{
    int width, height, numChannels; 
    GLuint id; 
    unsigned char* data = stbi_load(filePath, &width, &height, &numChannels, 0); 
    if(data == NULL)
    {
        logFile.log("load texture failed to load %s\n", filePath);
        return (0); 
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
        switch (numChannels) 
        {
        case 1:
            format = GL_RED;
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
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    
    stbi_image_free(data); 
    data = NULL; 

    return (id); 
} 

Model::Model() 
{
} 

int 
Model::initialize(const char* path) 
{
    logFile.log("Model::initialize() started\n"); 
    loadModel(path); 
    logFile.log("Model::initialize() completed successfully\n"); 
    return (0); 
} 

void 
Model::loadModel(const std::string& path) 
{
    logFile.log("Model::loadModel() started\n"); 
    Assimp::Importer importer; 

    directory = path.substr(0, path.find_last_of('/')); 

    const aiScene* scene = importer.ReadFile(path, 
            aiProcess_Triangulate		| // convert all faces to triangles 
            aiProcess_FlipUVs			| // flip texcoords for opengl 
            aiProcess_CalcTangentSpace	| // auto calculate tangents and bitangents 
            aiProcess_GenNormals			// generate normals if missing 
    ); 

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        logFile.log("Assimp error: %s\n", importer.GetErrorString()); 
        return; 
    } 


    int count(aiNode* node); 
    printf("nrnodes = \n"); //, count(scene->mRootNode)); 
    
    processNode(scene->mRootNode, scene, vmath::mat4::identity()); 

    logFile.log("Model::loadModel() completed successfully\n"); 
} 

int count(aiNode* node) 
{
    static int c = 0; 

    for(int i = 0; i < node->mNumChildren; ++i) 
    {
        c += 1; 
        count(node->mChildren[i]); 
    } 

    return (c); 
} 

void 
Model::processNode(aiNode* node, const aiScene* scene, vmath::mat4 parentTransform) 
{
    if(scene == nullptr)
    {
        logFile.log("Model::processNode() > Model loading failed\n"); 
        return;  
    } 

    if(node == nullptr) 
        return; 

    aiMatrix4x4 t = node->mTransformation; 
    vmath::mat4 nodeTransform = vmath::mat4(
        vec4(t.a1, t.b1, t.c1, t.d1), 
        vec4(t.a2, t.b2, t.c2, t.d2), 
        vec4(t.a3, t.b3, t.c3, t.d3), 
        vec4(t.a4, t.b4, t.c4, t.d4)  
    ); 

    vmath::mat4 globalTransform = parentTransform * nodeTransform; 

    for(unsigned int i = 0; i < node->mNumMeshes; ++i) 
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        Mesh m = processMesh(mesh, scene); 
        m.nodeTransform = globalTransform; 
        meshes.push_back(m); 
    } 

    for(unsigned int i = 0; i < node->mNumChildren; ++i) 
    {
        processNode(node->mChildren[i], scene, globalTransform); 
    } 
} 

Model::Mesh 
Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
    Mesh myMesh; 

    // extract vertices 
    for(unsigned int i = 0; i < mesh->mNumVertices; ++i) 
    {
        Vertex vertex; 

        vertex.position = vmath::vec3(
            mesh->mVertices[i].x, 
            mesh->mVertices[i].y, 
            mesh->mVertices[i].z  
        ); 

        vertex.normal = vmath::vec3(
            mesh->mNormals[i].x, 
            mesh->mNormals[i].y, 
            mesh->mNormals[i].z  
        ); 

        if(vertex.tangent != nullptr) 
        {
            vertex.tangent = vmath::vec3(
                mesh->mTangents[i].x, 
                mesh->mTangents[i].y, 
                mesh->mTangents[i].z  
            ); 
        } 

        if(vertex.bitangent != nullptr) 
        {
            vertex.bitangent = vmath::vec3(
                mesh->mBitangents[i].x, 
                mesh->mBitangents[i].y, 
                mesh->mBitangents[i].z  
            ); 
        } 

        if(mesh->mTextureCoords[0]) 
        {
            vertex.texCoord = vmath::vec2(
                mesh->mTextureCoords[0][i].x, 
                mesh->mTextureCoords[0][i].y
            ); 
        } 
        else 
        {
            vertex.texCoord = vmath::vec2(0.0f, 0.0f); 
        } 

        myMesh.vertices.push_back(vertex); 
    } 

    // extract indices 
    for(unsigned int i = 0; i < mesh->mNumFaces; ++i) 
    {
        aiFace face = mesh->mFaces[i]; 
        for(unsigned int j = 0; j < face.mNumIndices; j++) 
        {
            myMesh.indices.push_back(face.mIndices[j]); 
        } 
    } 

    // materials 
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; 

    std::vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse"); 
    std::vector<ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "normal"); 
    std::vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular"); 

    myMesh.textures.insert(myMesh.textures.end(), diffuseMaps.begin(), diffuseMaps.end()); 
    myMesh.textures.insert(myMesh.textures.end(), normalMaps.begin(), normalMaps.end()); 
    myMesh.textures.insert(myMesh.textures.end(), specularMaps.begin(), specularMaps.end()); 

    setupMesh(myMesh); 
    return (myMesh); 
} 

void 
Model::setupMesh(Model::Mesh& mesh) 
{
    glCreateVertexArrays(1, &mesh.vao); 
    glBindVertexArray(mesh.vao); 

    glGenBuffers(1, &mesh.vbo); 
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); 
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size()*sizeof(struct Vertex), mesh.vertices.data(), GL_STATIC_DRAW); 

    GLsizei stride = sizeof(struct Vertex); 
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0*sizeof(float))); 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float))); 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL); 
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(5*sizeof(float))); 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD); 
    glVertexAttribPointer(AMC_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8*sizeof(float))); 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TANGENT); 
    glVertexAttribPointer(AMC_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11*sizeof(float))); 
    glEnableVertexAttribArray(AMC_ATTRIBUTE_BITANGENT); 

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glGenBuffers(1, &mesh.ebo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size()*sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW); 

    glBindVertexArray(0); 
} 

void 
Model::draw(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix) 
{
    if(shaderProgramObject == 0) 
        assert(initShaderProgram() == 0); 

    glUseProgram(shaderProgramObject); 

    for(unsigned int i = 0; i < meshes.size(); ++i) 
    {
        vmath::mat4 mvpMatrix = _projectionMatrix * _viewMatrix * _modelMatrix * meshes[i].nodeTransform; 
        glUniformMatrix4fv(modelMatrixUniform, 1, GL_FALSE, _modelMatrix*meshes[i].nodeTransform); 
        glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, mvpMatrix); 

        if(meshes[i].textures.size() >= 1) 
        {
            glActiveTexture(GL_TEXTURE0); 
            glBindTexture(GL_TEXTURE_2D, meshes[i].textures[0].id); 
            glUniform1i(glGetUniformLocation(shaderProgramObject, "uDiffuse"), 0); 
        } 
        if(meshes[i].textures.size() >= 2) 
        {
            glActiveTexture(GL_TEXTURE1); 
            glBindTexture(GL_TEXTURE_2D, meshes[i].textures[1].id); 
            glUniform1i(glGetUniformLocation(shaderProgramObject, "uNormal"), 1); 
        } 
        if(meshes[i].textures.size() >= 3) 
        {
            glActiveTexture(GL_TEXTURE2); 
            glBindTexture(GL_TEXTURE_2D, meshes[i].textures[2].id); 
            glUniform1i(glGetUniformLocation(shaderProgramObject, "uSpecular"), 2); 
        } 

        glBindVertexArray(meshes[i].vao); 
        glDrawElements(GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0); 
    } 

    glUseProgram(0); 
} 

int 
Model::initShaderProgram(void) 
{
    // function declarations 
    void uninitialize(void); 

    // code 
    // ================= vertex shader ======================= 
    // 1) Write the shader source code 
    const GLchar* vertexShaderSourceCode = 
    "#version 460 core\n" \

    "in vec4 aPosition;\n" \
    "in vec2 aTexCoord;\n" \
    "in vec3 aNormal;\n" \
    
    "out vec2 out_texCoord;\n" \
    "out vec3 out_normal;\n" \
    "out vec3 out_fragPosition;\n" \
    
    "uniform mat4 uMVPMatrix;\n" \
    "uniform mat4 uModelMatrix;\n" \
    
    "void main(void) \n" \
    "{\n" \
    "	gl_Position = uMVPMatrix * aPosition;\n" \
    "	out_fragPosition = (uModelMatrix * aPosition).xyz;\n" \
    "	out_texCoord = aTexCoord;\n" \
    "	out_normal = mat3(transpose(inverse(uModelMatrix))) * aNormal;\n" \
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
                logFile.log("vertex shader compilation log = %s\n", szInfoLog);  
            
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
    "in vec3 out_fragPosition;\n" \

    "out vec4 FragColor;\n" \

    "uniform sampler2D uDiffuse;\n" \
    "uniform vec4 uLightPosition = vec4(100.0, 100.0, 100.0, 1.0);\n" \
    "uniform vec3 uLightColor = vec3(1.0, 1.0, 1.0);\n" \

    "void main(void)\n" \
    "{\n" \
    "	vec3 normalizedNormal = normalize(out_normal);\n" \
    "	vec3 lightDirection = normalize(uLightPosition.xyz - out_fragPosition);\n" \
    "	vec3 diffuse = max(dot(normalizedNormal, lightDirection), 0.0) * uLightColor;\n" \
    
    "	vec3 diffuseColor = texture(uDiffuse, out_texCoord).rgb;\n" \
    "	vec3 color = diffuseColor * diffuse;\n" \
    "	FragColor = vec4(color, 1.0);\n" \
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
                logFile.log("Fragment shader compilation log = %s\n", szInfoLog); 

                free(szInfoLog); 
                szInfoLog = NULL; 
            }
        }

        uninitialize(); 
    }

    // ================================= create, attach, link shader program object =========================== 
    // 1) Create shader program object 
    shaderProgramObject = glCreateProgram(); 

    // 2) attch shaders to this shader program object 
    glAttachShader(shaderProgramObject, vertexShaderObject); 

    // bind shader attribute at certain index in shader to save index in host program 
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
    glBindAttribLocation(shaderProgramObject, AMC_ATTRIBUTE_NORMAL, "aNormal");

    glAttachShader(shaderProgramObject, fragmentShaderObject); 

    // 3) tell to link shader objects to shader program objects 
    glLinkProgram(shaderProgramObject); 

    // 4) check for link error log 
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
                logFile.log("shader program object = %s\n", szInfoLog); 

                free(szInfoLog); 
                szInfoLog = NULL; 
            }
        }

        uninitialize(); 
    } 

    // ----- SENDING DATA INTO UNIFORM ---------- 
    mvpMatrixUniform = glGetUniformLocation(shaderProgramObject, "uMVPMatrix"); 
    modelMatrixUniform = glGetUniformLocation(shaderProgramObject, "uModelMatrix"); 

    return (0); 
} 

Model::~Model() 
{
    for(size_t i = 0; i < meshes.size(); ++i) 
    {
        if(meshes[i].ebo) 
        {
            glDeleteBuffers(1, &meshes[i].ebo); 
            meshes[i].ebo = 0; 
        } 
        if(meshes[i].vbo) 
        {
            glDeleteBuffers(1, &meshes[i].vbo); 
            meshes[i].vbo = 0; 
        } 
        if(meshes[i].vao) 
        {
            glDeleteVertexArrays(1, &meshes[i].vao); 
            meshes[i].vao = 0; 
        } 
    } 

    for(size_t i = 0; i < loadedTextures.size(); ++i) 
        glDeleteTextures(1, &loadedTextures[i].id); 
    loadedTextures.clear(); 

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

        glUseProgram(0); 
        glDeleteProgram(shaderProgramObject);   
    }
} 
