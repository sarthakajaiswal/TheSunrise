#include "..\headers\butterfly.hpp" 

Butterfly::Butterfly() 
{
	shaderProgram = 0;
	texture = 0; 
} 

int 
Butterfly::initialize(const char* wingTextureImagePath) 
{
    logFile.log("Butterfly::initialize() > initialize started\n"); 
	quad.initialize(); 
	texture = loadTexture(wingTextureImagePath, TRUE); 

    initShaderProgram(); 

    logFile.log("Butterfly::initialize() > initialize completed successfully\n"); 
	return (0); 
} 

void 
Butterfly::initShaderProgram(void) 
{
    // function declarations 
    void uninitialize(void); 

    // code 
    const GLchar* vertexShaderSourceCode = 
        "#version 460 core\n" \

        "in vec4 aPosition;\n" \
        "in vec2 aTexCoord;\n" \
        "uniform mat4 uMVPMatrix;\n" \
        
        "out vec2 out_texCoord;\n" \

        "void main(void) \n" \
        "{\n" \
        "   out_texCoord = aTexCoord;\n" \
        "	gl_Position = uMVPMatrix * aPosition;\n" \
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
                logFile.log("Butterfly vertex shader compilation log = %s\n", szInfoLog);  
            
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
        "out vec4 FragColor;\n" \

		"layout (binding=0) uniform sampler2D uTexture;\n" \

        "void main(void)\n" \
        "{\n" \
        "	FragColor = texture(uTexture, out_texCoord);\n" \
		"	if(FragColor.a < 0.01)\n" \
		"		discard;\n" \
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
                logFile.log("Butterfly shaderProgram Fragment shader compilation log = %s\n", szInfoLog); 

                free(szInfoLog); 
                szInfoLog = NULL; 
            }
        }

        uninitialize(); 
    }

    shaderProgram = glCreateProgram(); 

    glAttachShader(shaderProgram, vertexShaderObject); 
    glBindAttribLocation(shaderProgram, AMC_ATTRIBUTE_POSITION, "aPosition");
    glBindAttribLocation(shaderProgram, AMC_ATTRIBUTE_TEXCOORD, "aTexCoord");
    glAttachShader(shaderProgram, fragmentShaderObject); 

    glLinkProgram(shaderProgram); 

    // check for link error log 
    status = 0; 
    infoLogLength = 0; 
    szInfoLog = 0; 
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status); 
    if(status == GL_FALSE) 
    {
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength); 
        if(infoLogLength > 0) 
        {
            szInfoLog = (GLchar*)malloc(infoLogLength * sizeof(GLchar)); 
            if(szInfoLog != NULL) 
            {
                glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, szInfoLog); 
                logFile.log("shaderProgram = %s\n", szInfoLog); 

                free(szInfoLog); 
                szInfoLog = NULL; 
            }
        }

        uninitialize(); 
    } 

    // ----- SENDING DATA INTO UNIFORM ---------- 
    mvpMatrixUniform = glGetUniformLocation(shaderProgram, "uMVPMatrix");
} 

void 
Butterfly::render(vmath::mat4 _modelMatrix, vmath::mat4 _viewMatrix, vmath::mat4 _projectionMatrix) 
{
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glUseProgram(shaderProgram); 
	
	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, texture); 
	
	vmath::mat4 modelMatrix = _modelMatrix; // push matrix 
	vmath::mat4 rotationMatrix = vmath::rotate(85.0f+wingAngle, 0.0f, 1.0f, 0.0f); 
	modelMatrix *= rotationMatrix; 
	modelMatrix *= vmath::translate(-1.0f, 0.0f, 0.0f);
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, _projectionMatrix * _viewMatrix * modelMatrix); 
	quad.render(); 

	modelMatrix = _modelMatrix; // pop matrix 
	rotationMatrix = vmath::rotate(-(85.0f+wingAngle), 0.0f, 1.0f, 0.0f); 
	modelMatrix *= rotationMatrix; 
	modelMatrix *= vmath::translate(1.0f, 0.0f, 0.0f);
	modelMatrix *= vmath::scale(-1.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(mvpMatrixUniform, 1, GL_FALSE, _projectionMatrix * _viewMatrix * modelMatrix); 
	quad.render(); 

	glUseProgram(0); 

	glDisable(GL_BLEND); 
} 

void 
Butterfly::update(float wingSpeed) 
{
	static float theta = 0.0f; 
	wingAngle = -10.0 * (sin(theta)+1.0f); 
	theta += wingSpeed/100.0f;
} 

Butterfly::~Butterfly() 
{
	if(texture != 0) 
	{
		glDeleteTextures(1, &texture); 
		texture = 0; 
	} 
} 

