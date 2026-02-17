#include "..//headers/objects.hpp"  

Cube::Cube() 
{
	vao = 0; 
	vbo_position  = 0; 
	vbo_normal = 0; 
	vbo_texCoord = 0; 
} 

int Cube::initialize() 
{
    // // variable declarations 
    const float cube_vertices[] = 
    {
        // front
        1.0f,  1.0f,  1.0f, // top-right of front
        -1.0f,  1.0f,  1.0f, // top-left of front
        -1.0f, -1.0f,  1.0f, // bottom-left of front
        1.0f, -1.0f,  1.0f, // bottom-right of front

        // right
        1.0f,  1.0f, -1.0f, // top-right of right
        1.0f,  1.0f,  1.0f, // top-left of right
        1.0f, -1.0f,  1.0f, // bottom-left of right
        1.0f, -1.0f, -1.0f, // bottom-right of right

        // back
        1.0f,  1.0f, -1.0f, // top-right of back
        -1.0f,  1.0f, -1.0f, // top-left of back
        -1.0f, -1.0f, -1.0f, // bottom-left of back
        1.0f, -1.0f, -1.0f, // bottom-right of back

        // left
        -1.0f,  1.0f,  1.0f, // top-right of left
        -1.0f,  1.0f, -1.0f, // top-left of left
        -1.0f, -1.0f, -1.0f, // bottom-left of left
        -1.0f, -1.0f,  1.0f, // bottom-right of left

        // top
        1.0f,  1.0f, -1.0f, // top-right of top
        -1.0f,  1.0f, -1.0f, // top-left of top
        -1.0f,  1.0f,  1.0f, // bottom-left of top
        1.0f,  1.0f,  1.0f, // bottom-right of top

        // bottom
        1.0f, -1.0f,  1.0f, // top-right of bottom
        -1.0f, -1.0f,  1.0f, // top-left of bottom
        -1.0f, -1.0f, -1.0f, // bottom-left of bottom
        1.0f, -1.0f, -1.0f // bottom-right of bottom
    }; 

	const GLfloat cube_color[] = 
	{
		// front
		1.0f, 0.0f, 0.0f, // top-right of front
		1.0f, 0.0f, 0.0f, // top-left of front
		1.0f, 0.0f, 0.0f, // bottom-left of front
		1.0f, 0.0f, 0.0f, // bottom-right of front

		// right
		0.0f, 0.0f, 1.0f, // top-right of right
		0.0f, 0.0f, 1.0f, // top-left of right
		0.0f, 0.0f, 1.0f, // bottom-left of right
		0.0f, 0.0f, 1.0f, // bottom-right of right

		// back
		1.0f, 1.0f, 0.0f, // top-right of back
		1.0f, 1.0f, 0.0f, // top-left of back
		1.0f, 1.0f, 0.0f, // bottom-left of back
		1.0f, 1.0f, 0.0f, // bottom-right of back

		// left
		1.0f, 0.0f, 1.0f, // top-right of left
		1.0f, 0.0f, 1.0f, // top-left of left
		1.0f, 0.0f, 1.0f, // bottom-left of left
		1.0f, 0.0f, 1.0f, // bottom-right of left

		// top
		0.0f, 1.0f, 0.0f, // top-right of top
		0.0f, 1.0f, 0.0f, // top-left of top
		0.0f, 1.0f, 0.0f, // bottom-left of top
		0.0f, 1.0f, 0.0f, // bottom-right of top

		// bottom
		1.0f, 0.5f, 0.0f, // top-right of bottom
		1.0f, 0.5f, 0.0f, // top-left of bottom
		1.0f, 0.5f, 0.0f, // bottom-left of bottom
		1.0f, 0.5f, 0.0f, // bottom-right of bottom
	}; 

    const GLfloat cube_normals[] = 
    {
        // front surface
        0.0f,  0.0f,  1.0f, // top-right of front
        0.0f,  0.0f,  1.0f, // top-left of front
        0.0f,  0.0f,  1.0f, // bottom-left of front
        0.0f,  0.0f,  1.0f, // bottom-right of front

        // right surface
        1.0f,  0.0f,  0.0f, // top-right of right
        1.0f,  0.0f,  0.0f, // top-left of right
        1.0f,  0.0f,  0.0f, // bottom-left of right
        1.0f,  0.0f,  0.0f, // bottom-right of right

        // back surface
        0.0f,  0.0f, -1.0f, // top-right of back
        0.0f,  0.0f, -1.0f, // top-left of back
        0.0f,  0.0f, -1.0f, // bottom-left of back
        0.0f,  0.0f, -1.0f, // bottom-right of back

        // left surface
        -1.0f,  0.0f,  0.0f, // top-right of left
        -1.0f,  0.0f,  0.0f, // top-left of left
        -1.0f,  0.0f,  0.0f, // bottom-left of left
        -1.0f,  0.0f,  0.0f, // bottom-right of left

        // top surface
        0.0f,  1.0f,  0.0f, // top-right of top
        0.0f,  1.0f,  0.0f, // top-left of top
        0.0f,  1.0f,  0.0f, // bottom-left of top
        0.0f,  1.0f,  0.0f, // bottom-right of top

        // bottom surface
        0.0f, -1.0f,  0.0f, // top-right of bottom
        0.0f, -1.0f,  0.0f, // top-left of bottom
        0.0f, -1.0f,  0.0f, // bottom-left of bottom
        0.0f, -1.0f,  0.0f // bottom-right of bottom
    }; 

    const GLfloat cube_texCoords[] =
    {
        // front
        1.0f, 1.0f, // top-right of front
        0.0f, 1.0f, // top-left of front
        0.0f, 0.0f, // bottom-left of front
        1.0f, 0.0f, // bottom-right of front

        // right
        1.0f, 1.0f, // top-right of right
        0.0f, 1.0f, // top-left of right
        0.0f, 0.0f, // bottom-left of right
        1.0f, 0.0f, // bottom-right of right

        // back
        1.0f, 1.0f, // top-right of back
        0.0f, 1.0f, // top-left of back
        0.0f, 0.0f, // bottom-left of back
        1.0f, 0.0f, // bottom-right of back

        // left
        1.0f, 1.0f, // top-right of left
        0.0f, 1.0f, // top-left of left
        0.0f, 0.0f, // bottom-left of left
        1.0f, 0.0f, // bottom-right of left

        // top
        1.0f, 1.0f, // top-right of top
        0.0f, 1.0f, // top-left of top
        0.0f, 0.0f, // bottom-left of top
        1.0f, 0.0f, // bottom-right of top

        // bottom
        1.0f, 1.0f, // top-right of bottom
        0.0f, 1.0f, // top-left of bottom
        0.0f, 0.0f, // bottom-left of bottom
        1.0f, 0.0f // bottom-right of bottom
    };

    // vertex array object for arrays of vertex attributes 
    glGenVertexArrays(1, &this->vao); // vao-vertex array object 
    glBindVertexArray(this->vao); 
    
    // position 
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

	// color 
    glGenBuffers(1, &vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_color), cube_color, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // normal vbo
    glGenBuffers(1, &vbo_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texCoords vbo
    glGenBuffers(1, &vbo_texCoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_texCoords), cube_texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);  

	return (0); 
} 

void Cube::render() 
{
    glBindVertexArray(vao); 
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4); 
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4); 
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4); 
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4); 
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4); 
	glBindVertexArray(0); 
} 

Cube::~Cube() 
{
    // code 
    // free vbo 
	if(vbo_texCoord) 
	{
		glDeleteBuffers(1, &vbo_texCoord); 
		vbo_texCoord = 0; 
	}
	if(vbo_normal) 
	{
		glDeleteBuffers(1, &vbo_normal); 
		vbo_normal = 0; 
	}
	if(vbo_color) 
	{
		glDeleteBuffers(1, &vbo_color); 
		vbo_color = 0; 
	}
	if(vbo_position) 
	{
		glDeleteBuffers(1, &vbo_position); 
		vbo_position = 0; 
	}

	// free vao 
	if(vao) 
	{
		glDeleteVertexArrays(1, &vao); 
		vao = 0; 
	}
} 

// ----------- 

Triangle::Triangle() 
{
	vao = 0; 
	vbo_position = 0; 
	vbo_color = 0; 
	vbo_texCoord = 0; 
} 

int Triangle::initialize() 
{
    // variable declarations 
	// provide vertex position, color, normal, texCoord, etc... 
	const GLfloat triangle_position[] = 
	{
		0.0f, 1.0f, 0.0f, 
		-1.0f, -1.0f, 0.0f, 
		1.0f, -1.0f, 0.0f 
	}; 

	GLfloat triangle_color[] = 
    {
        1.0, 0.0, 0.0, 
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    }; 

	GLfloat triangle_texCoords[] = 
	{
		0.5f, 1.0f, 
		0.0f, 0.0f, 
		0.0f, 1.0f
	}; 

	// code 
    // vertex array object for arrays of vertex attributes 
    glGenVertexArrays(1, &vao); // vao-vertex array object 
    glBindVertexArray(vao); 
    
    // position  
    glGenBuffers(1, &vbo_position); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_position), triangle_position, GL_STATIC_DRAW); 
        glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
        glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // color 
    glGenBuffers(1, &vbo_color); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_color), triangle_color, GL_STATIC_DRAW); 
        glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, 0, NULL); 
        glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // texCoords 
    glGenBuffers(1, &vbo_texCoord); 
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoord); 
        glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_texCoords), triangle_texCoords, GL_STATIC_DRAW); 
        glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
        glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

	return (0); 
} 

void Triangle::render() 
{
    // code 
	glBindVertexArray(vao); 
	glDrawArrays(GL_TRIANGLES, 0, 3); 
	glBindVertexArray(0); 
} 

Triangle::~Triangle() 
{
	// code 
	// free vbo 
	if(vbo_texCoord) 
	{
		glDeleteBuffers(1, &vbo_texCoord); 
		vbo_texCoord = 0;
	} 
	if(vbo_color) 
	{
		glDeleteBuffers(1, &vbo_color); 
		vbo_color = 0; 
	}
	if(vbo_position) 
	{
		glDeleteBuffers(1, &vbo_position); 
		vbo_position = 0; 
	}

	// free vao 
	if(vao) 
	{
		glDeleteVertexArrays(1, &vao); 
		vao = 0; 
	}
} 

// ------------ 

Quad::Quad() 
{
	vao = 0; 
	vbo_position = 0; 
	vbo_texCoord = 0; 
	vbo_normal = 0; 
} 

int Quad::initialize() 
{
	// variable declarations 
	vec3 pos1 = vec3(1.0, 1.0, 0.0); 
	vec3 pos2 = vec3(-1.0, 1.0, 0.0); 
	vec3 pos3 = vec3(-1.0, -1.0, 0.0); 
	vec3 pos4 = vec3(1.0, -1.0, 0.0); 

	const GLfloat rectangle_position[] = 
	{
		1.0, 1.0, 0.0, 
		-1.0, 1.0, 0.0, 
		-1.0, -1.0, 0.0, 
		1.0, -1.0, 0.0
	}; 

	const GLfloat rectangle_texcoord[] = 
	{
		1.0, 1.0, 
		0.0, 1.0, 
		0.0, 0.0, 
		1.0, 0.0
	};  

	const GLfloat rectangle_normals[] = 
	{
		0.0, 0.0, 1.0f, 
		0.0, 0.0, 1.0f, 
		0.0, 0.0, 1.0f, 
		0.0, 0.0, 0.0f 
	};  

	// // calculate tangent/bitangent vectors of both triangles
	// vec3 tangent1, bitangent1;
	// vec3 tangent2, bitangent2;
	
	// // triangle 1
	// vec3 edge1 = vec3(rectangle_position[3], rectangle_position[4], rectangle_position[5]) - vec3(rectangle_position[0], rectangle_position[1], rectangle_position[2]); 
	// vec3 edge2 = vec3(rectangle_position[6], rectangle_position[7], rectangle_position[8]) - vec3(rectangle_position[0], rectangle_position[1], rectangle_position[2]); 
	// vec2 deltaUV1 = vec2(rectangle_texcoord[2] - rectangle_texcoord[3]) - vec2(rectangle_texcoord[0], rectangle_texcoord[1]);
	// vec2 deltaUV2 = vec2(rectangle_texcoord[4] - rectangle_texcoord[5]) - vec2(rectangle_texcoord[0], rectangle_texcoord[1]);

	// float f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

	// tangent1[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
	// tangent1[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
	// tangent1[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);

	// bitangent1[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
	// bitangent1[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
	// bitangent1[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);

	// // triangle 2
	// edge1 = vec3(rectangle_position[6], rectangle_position[7], rectangle_position[8]) - vec3(rectangle_position[0], rectangle_position[1], rectangle_position[2]); 
	// edge2 = vec3(rectangle_position[9], rectangle_position[10], rectangle_position[11]) - vec3(rectangle_position[0], rectangle_position[1], rectangle_position[2]); 
	// deltaUV1 = vec2(rectangle_texcoord[4] - rectangle_texcoord[5]) - vec2(rectangle_texcoord[0], rectangle_texcoord[1]);
	// deltaUV2 = vec2(rectangle_texcoord[6] - rectangle_texcoord[7]) - vec2(rectangle_texcoord[0], rectangle_texcoord[1]);

	// f = 1.0f / (deltaUV1[0] * deltaUV2[1] - deltaUV2[0] * deltaUV1[1]);

	// tangent2[0] = f * (deltaUV2[1] * edge1[0] - deltaUV1[1] * edge2[0]);
	// tangent2[1] = f * (deltaUV2[1] * edge1[1] - deltaUV1[1] * edge2[1]);
	// tangent2[2] = f * (deltaUV2[1] * edge1[2] - deltaUV1[1] * edge2[2]);

	// bitangent2[0] = f * (-deltaUV2[0] * edge1[0] + deltaUV1[0] * edge2[0]);
	// bitangent2[1] = f * (-deltaUV2[0] * edge1[1] + deltaUV1[0] * edge2[1]);
	// bitangent2[2] = f * (-deltaUV2[0] * edge1[2] + deltaUV1[0] * edge2[2]);

	// code 
    // vertex array object for arrays of vertex attributes 
    glGenVertexArrays(1, &vao); // vao-vertex array object 
    glBindVertexArray(vao); 
    
    // position 
    glGenBuffers(1, &vbo_position);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_position);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_position), rectangle_position, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texcoord vbo
    glGenBuffers(1, &vbo_texCoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_texcoord), rectangle_texcoord, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // texcoord vbo
    glGenBuffers(1, &vbo_normal);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_normals), rectangle_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // tangent vbo
    // glGenBuffers(1, &vbo_tangent);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_tangent);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_normals), rectangle_normals, GL_STATIC_DRAW);
    // glVertexAttribPointer(AMC_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // glEnableVertexAttribArray(AMC_ATTRIBUTE_TANGENT);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // // bitangent vbo
    // glGenBuffers(1, &vbo_bitangent);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangent);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle_normals), rectangle_normals, GL_STATIC_DRAW);
    // glVertexAttribPointer(AMC_ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    // glEnableVertexAttribArray(AMC_ATTRIBUTE_BITANGENT);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0); 
	
	return (0); 
} 

void Quad::render() 
{
	// code 
	glBindVertexArray(vao); 
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	glBindVertexArray(0); 
} 

Quad::~Quad() 
{
	// code 
	// free vbo 
	if(vbo_texCoord) 
	{
		glDeleteBuffers(1, &vbo_texCoord); 
		vbo_texCoord = 0;
	} 
	if(vbo_normal) 
	{
		glDeleteBuffers(1, &vbo_normal); 
		vbo_normal = 0; 
	}
	if(vbo_position) 
	{
		glDeleteBuffers(1, &vbo_position); 
		vbo_position = 0; 
	}

	// free vao 
	if(vao) 
	{
		glDeleteVertexArrays(1, &vao); 
		vao = 0; 
	}
} 

// ------------ 

/* 
// skybox 
static GLuint vao_skybox = 0; 
static GLuint vbo_skybox = 0; 
static GLuint ebo_skybox = 0; 
static bool isSkyboxInitialised = false; 
int initSkybox(void) 
{
    // variable declarations 
    GLfloat skyboxVertices[] =
	{
		// position + texcoord
		1.0f, 1.0f, 1.0f, 		1.0f, 1.0f, 1.0f,       // 0
		-1.0f, 1.0f, 1.0f, 		-1.0f, 1.0f, 1.0f,     // 1
		-1.0f, -1.0f, 1.0f, 	-1.0f, -1.0f, 1.0f,   // 2
		1.0f, -1.0f, 1.0f, 		1.0f, -1.0f, 1.0f,     // 3
		1.0f, 1.0f, -1.0f, 		1.0f, 1.0f, -1.0f,     // 4
		-1.0f, 1.0f, -1.0f, 	-1.0f, 1.0f, -1.0f,   // 5
		-1.0f, -1.0f, -1.0f, 	-1.0f, -1.0f, -1.0f, // 6
		1.0f, -1.0f, -1.0f, 	1.0f, -1.0f, -1.0f,   // 7
	};

    GLushort skyboxIndices[] =
	{
		// front face
		0, 1, 3,
		3, 1, 2,

		// right face
		4, 0, 7,
		7, 0, 3,

		// back face
		4, 5, 7,
		7, 5, 6,

		// left face
		1, 5, 2,
		2, 5, 6,

		// top face
		4, 5, 0,
		0, 5, 1,

		// bottom face
		3, 2, 7,
		7, 2, 6
	};

    // code 
    if(isSkyboxInitialised == false)
    {
        // vao
		glGenVertexArrays(1, &vao_skybox);
		glBindVertexArray(vao_skybox);

		// VBO for position + texcoord
		glGenBuffers(1, &vbo_skybox);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_skybox);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, NULL);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);

		glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (const void *)(sizeof(GLfloat) * 3));
		glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// ebo for indices 
		glGenBuffers(1, &ebo_skybox);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_skybox);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

		glBindVertexArray(0);

        isSkyboxInitialised = true; 
    } 
} 

void renderSkybox(void) 
{
    // code 
    // draw the vertex arrays 
	glBindVertexArray(vao_skybox); 
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0); 
} 

void uninitializeSkybox(void) 
{
    // code 
    // free vbo 
	if(ebo_skybox) 
	{
		glDeleteBuffers(1, &ebo_skybox); 
		ebo_skybox = 0; 
	}
	if(vbo_skybox) 
	{
		glDeleteBuffers(1, &vbo_skybox); 
		vbo_skybox = 0; 
	}

	// free vao 
	if(vao_skybox) 
	{
		glDeleteVertexArrays(1, &vao_skybox); 
		vao_skybox = 0; 
	}
} 

// sphere 
static GLuint vao_sphere = 0; 
static GLuint vbo_position_sphere = 0; 
static GLuint vbo_normal_sphere = 0; 
static GLuint vbo_texCoord_sphere = 0; 
static GLuint vbo_element_sphere = 0; 

static float sphere_vertices[1146];
static float sphere_normals[1146];
static float sphere_textures[764];
static unsigned short sphere_elements[2280];
static int gNumElements, gNumVertices; 
static bool isSphereInitialized = false; 
int initSphere(void) 
{
    // code 
	if(isSphereInitialized == false) 
	{
		getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
		gNumVertices = getNumberOfSphereVertices();
		gNumElements = getNumberOfSphereElements();

		// vertex array object for arrays of vertex attributes 
		glGenVertexArrays(1, &vao_sphere); // vao-vertex array object 
		glBindVertexArray(vao_sphere); 
		
		// position 
		glGenBuffers(1, &vbo_position_sphere);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_position_sphere);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_vertices), sphere_vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// texCoords 
		glGenBuffers(1, &vbo_texCoord_sphere);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_texCoord_sphere);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_textures), sphere_textures, GL_STATIC_DRAW);
		glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// normal vbo
		glGenBuffers(1, &vbo_normal_sphere);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_sphere);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_normals), sphere_normals, GL_STATIC_DRAW);
		glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// element vbo
		glGenBuffers(1, &vbo_element_sphere);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_element_sphere);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere_elements), sphere_elements, GL_STATIC_DRAW);
		glEnableVertexAttribArray(AMC_ATTRIBUTE_ELEMENT);

		glBindVertexArray(0); 

		isSphereInitialized = true; 
	} 

	return (0); 
} 

void renderSphere(void) 
{
    // code 
    glBindVertexArray(vao_sphere); 
    glDrawElements(GL_TRIANGLES, gNumElements, GL_UNSIGNED_SHORT, 0); 
	glBindVertexArray(0); 
} 

void uninitializeSphere(void) 
{
    // code 
    if(vbo_element_sphere) 
	{
		glDeleteBuffers(1, &vbo_element_sphere); 
		vbo_element_sphere = 0; 
	}
	if(vbo_normal_sphere) 
	{
		glDeleteBuffers(1, &vbo_normal_sphere); 
		vbo_normal_sphere = 0; 
	}
	// if(vbo_texCoord_sphere) 
	// {
	// 	glDeleteBuffers(1, &vbo_texCoord_sphere); 
	// 	vbo_texCoord_sphere = 0; 
	// }
	if(vbo_position_sphere) 
	{
		glDeleteBuffers(1, &vbo_position_sphere); 
		vbo_position_sphere = 0; 
	} 
	if(vao_sphere) 
	{
		glDeleteVertexArrays(1, &vao_sphere); 
		vao_sphere = 0; 
	} 

	isSphereInitialized = false; 
} 
*/ 