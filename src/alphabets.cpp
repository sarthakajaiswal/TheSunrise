#include "..\headers\alphabets.hpp" 

const int DEPTH_OF_ALPHABETS = 1.0f; 

// -------- A ------------- 
const int numQuadsInA = 2; 
static struct AlphabetQuad quadsOfA[numQuadsInA] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
    {0.15f, 1.0f, -0.15f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, {10.0f, 0.0f, 0.0f, 1.0f}}, 
    {0.15f, 1.0f, -0.15f, 1.0f, 0.7f, -1.0f, 1.0f, -1.0f, {10.0f, 0.0f, 0.0f, 1.0f}}
}; 
GLuint vaosA[numQuadsInA]; 
GLuint vbosA[numQuadsInA];

void initAlphabet_A() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfA)/sizeof(quadsOfA[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfA+i, DEPTH_OF_ALPHABETS, vaosA+i, vbosA+i); 
}  

void renderAlphabet_A() 
{
    for(size_t i = 0; i < numQuadsInA; ++i) 
    {
        glBindVertexArray(vaosA[i]); 
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4); 
        glBindVertexArray(0); 
    } 
}  

void uninitializeAlphabet_A() 
{
    if(vbosA[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInA, vbosA); 
        memset((void*)vbosA, 0, sizeof(vbosA)); 
    } 

    if(vaosA[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInA, vaosA); 
        memset((void*)vaosA, 0, sizeof(vaosA)); 
    } 
}  

// ===================== helper routines ===================== 
void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo) 
{
    float z = depth/2.0f; 

	GLfloat vertices[] = {
		// position 				// color										// texCoord 	// tanget 	// normal 									// normal  
		quad->p1x, quad->p1y, z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	0.0f, 0.0f, 1.0f, 	
		quad->p2x, quad->p2y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	0.0f, 0.0f, 1.0f, 	
		quad->p3x, quad->p3y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	0.0f, 0.0f, 1.0f, 	
		quad->p4x, quad->p4y, z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0,  	0.0f, 0.0f, 1.0f,	

		quad->p1x, quad->p1y, -z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	1.0f, 0.0f, 0.0f, 	
		quad->p1x, quad->p1y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	1.0f, 0.0f, 0.0f, 	
		quad->p4x, quad->p4y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	1.0f, 0.0f, 0.0f, 	
		quad->p4x, quad->p4y, -z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0,  	1.0f, 0.0f, 0.0f, 	
		
		quad->p2x, quad->p2y, -z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	0.0f, 0.0f, -1.0f, 	
		quad->p1x, quad->p1y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	0.0f, 0.0f, -1.0f, 	
		quad->p4x, quad->p4y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	0.0f, 0.0f, -1.0f, 	
		quad->p3x, quad->p3y, -z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0,  	0.0f, 0.0f, -1.0f, 	
		
		quad->p2x, quad->p2y, z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	-1.0f, 0.0f, 0.0f, 	
		quad->p2x, quad->p2y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	-1.0f, 0.0f, 0.0f, 	
		quad->p3x, quad->p3y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	-1.0f, 0.0f, 0.0f, 	
		quad->p3x, quad->p3y, z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0,  	-1.0f, 0.0f, 0.0f, 	
		
		quad->p1x, quad->p1y, -z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	0.0f, 1.0f, 0.0f, 	
		quad->p2x, quad->p2y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	0.0f, 1.0f, 0.0f, 	
		quad->p2x, quad->p2y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	0.0f, 1.0f, 0.0f, 	
		quad->p1x, quad->p1y, z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0,  	0.0f, 1.0f, 0.0f, 	
		
		quad->p4x, quad->p4y, z,	quad->color.r, quad->color.g, quad->color.b,	1.0f, 1.0f,		1,0,0,  	0.0f, -1.0f, 0.0f, 	
		quad->p3x, quad->p3y, z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 1.0f,		1,0,0,  	0.0f, -1.0f, 0.0f, 	
		quad->p3x, quad->p3y, -z, 	quad->color.r, quad->color.g, quad->color.b,	0.0f, 0.0f,		1,0,0,  	0.0f, -1.0f, 0.0f, 	
		quad->p4x, quad->p4y, -z, 	quad->color.r, quad->color.g, quad->color.b,	1.0f, 0.0f,		1,0,0, 		0.0f, -1.0f, 0.0f,	
	}; 

	float stride = 14*sizeof(float); 

	// code 
	glCreateVertexArrays(1, vao); 
	glBindVertexArray(*vao); 

	glCreateBuffers(1, vbo); 
	glBindBuffer(GL_ARRAY_BUFFER, *vbo); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW); 
	
	glVertexAttribPointer(AMC_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_POSITION); 
	
	glVertexAttribPointer(AMC_ATTRIBUTE_COLOR, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_COLOR); 

	glVertexAttribPointer(AMC_ATTRIBUTE_TEXCOORD, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TEXCOORD); 

	glVertexAttribPointer(AMC_ATTRIBUTE_TANGENT, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_TANGENT); 

	glVertexAttribPointer(AMC_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, (void*)(11*sizeof(float))); 
	glEnableVertexAttribArray(AMC_ATTRIBUTE_NORMAL); 
	
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 
} 
