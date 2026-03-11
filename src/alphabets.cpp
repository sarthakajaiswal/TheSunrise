#include "..\headers\alphabets.hpp" 

const float DEPTH_OF_ALPHABETS = 0.6f; 
const Color alphabetColor = {10.0f, 0.0f, 0.0f, 1.0f}; 

// -------- A ------------- 
const int numQuadsInA = 2; 
static struct AlphabetQuad quadsOfA[numQuadsInA] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
    {0.15f, 1.0f, -0.15f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
    {0.15f, 1.0f, -0.15f, 1.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}
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

void renderAlphabet_A(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosA, numQuadsInA); 
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

// --------- S ---------- 
const int numQuadsInS = 5; 
static struct AlphabetQuad quadsOfS[numQuadsInS] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, 0.0f, -0.7f, 0.0f, alphabetColor}, 
        {1.0f, 0.15f, -1.0f, 0.15f, -1.0f, -0.15f, 1.0f, -0.15f, alphabetColor}, 
        {1.0f, 0.0f, 0.7f, 0.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor}
    };
GLuint vaosS[numQuadsInS]; 
GLuint vbosS[numQuadsInS];

void initAlphabet_S() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfS)/sizeof(quadsOfS[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfS+i, DEPTH_OF_ALPHABETS, vaosS+i, vbosS+i); 
}  

void renderAlphabet_S(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosS, numQuadsInS); 
}  

void uninitializeAlphabet_S() 
{
    if(vbosS[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInS, vbosS); 
        memset((void*)vbosS, 0, sizeof(vbosS)); 
    } 

    if(vaosS[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInS, vaosS); 
        memset((void*)vaosS, 0, sizeof(vaosS)); 
    } 
} 

// --------- T ---------- 
const int numQuadsInT = 2; 
static struct AlphabetQuad quadsOfT[numQuadsInT] = {
    	// p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {0.15f, 1.0f, -0.15f, 1.0f, -0.15f, -1.0f, 0.15f, -1.0f, alphabetColor}
    }; 
GLuint vaosT[numQuadsInT]; 
GLuint vbosT[numQuadsInT];

void initAlphabet_T() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfT)/sizeof(quadsOfT[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfT+i, DEPTH_OF_ALPHABETS, vaosT+i, vbosT+i); 
}  

void renderAlphabet_T(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosT, numQuadsInT); 
}  

void uninitializeAlphabet_T() 
{
    if(vbosT[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInT, vbosT); 
        memset((void*)vbosT, 0, sizeof(vbosT)); 
    } 

    if(vaosT[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInT, vaosT); 
        memset((void*)vaosT, 0, sizeof(vaosT)); 
    } 
} 

// --------- R ---------- 
const int numQuadsInR = 5; 
static struct AlphabetQuad quadsOfR[numQuadsInR] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {1.0f, 1.0f, 0.7f, 1.0f, 0.7f, 0.0f, 1.0f, 0.0f, alphabetColor}, 
        {1.0f, 0.15f, -1.0f, 0.15f, -1.0f, -0.15f, 1.0f, -0.15f, alphabetColor}, 
        {0.5f, 0.0f, 0.2f, 0.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}  
    }; 
GLuint vaosR[numQuadsInR]; 
GLuint vbosR[numQuadsInR];

void initAlphabet_R() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfR)/sizeof(quadsOfR[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfR+i, DEPTH_OF_ALPHABETS, vaosR+i, vbosR+i); 
}  

void renderAlphabet_R(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosR, numQuadsInR); 
}  

void uninitializeAlphabet_R() 
{
    if(vbosR[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInR, vbosR); 
        memset((void*)vbosR, 0, sizeof(vbosR)); 
    } 

    if(vaosR[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInR, vaosR); 
        memset((void*)vaosR, 0, sizeof(vaosS)); 
    } 
} 

// --------- O ---------- 
const int numQuadsInO = 4; 
static struct AlphabetQuad quadsOfO[numQuadsInO] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, 0.7f, 1.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}  
    }; 
GLuint vaosO[numQuadsInO]; 
GLuint vbosO[numQuadsInO];

void initAlphabet_O() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfO)/sizeof(quadsOfO[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfO+i, DEPTH_OF_ALPHABETS, vaosO+i, vbosO+i); 
}  

void renderAlphabet_O(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosO, numQuadsInO); 
}  

void uninitializeAlphabet_O() 
{
    if(vbosO[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInO, vbosO); 
        memset((void*)vbosO, 0, sizeof(vbosO)); 
    } 

    if(vaosO[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInO, vaosO); 
        memset((void*)vaosO, 0, sizeof(vaosO)); 
    } 
} 

// --------- M ---------- 
const int numQuadsInM = 4; 
static struct AlphabetQuad quadsOfM[numQuadsInM] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {0.15f, 1.0f, -0.15f, 1.0f, -0.15f, -1.0f, 0.15f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, 0.7f, 1.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}  
    }; 
GLuint vaosM[numQuadsInM]; 
GLuint vbosM[numQuadsInM];

void initAlphabet_M() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfM)/sizeof(quadsOfM[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfM+i, DEPTH_OF_ALPHABETS, vaosM+i, vbosM+i); 
}  

void renderAlphabet_M(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosM, numQuadsInM); 
}  

void uninitializeAlphabet_M() 
{
    if(vbosM[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInM, vbosM); 
        memset((void*)vbosM, 0, sizeof(vbosM)); 
    } 

    if(vaosM[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInM, vaosM); 
        memset((void*)vaosM, 0, sizeof(vaosM)); 
    } 
} 

// --------- E ---------- 
const int numQuadsInE = 4; 
static struct AlphabetQuad quadsOfE[numQuadsInE] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {0.0f, 0.15f, -1.0f, 0.15f, -1.0f, -0.15f, 0.0f, -0.15f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor}
    }; 
GLuint vaosE[numQuadsInE]; 
GLuint vbosE[numQuadsInE];

void initAlphabet_E() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfE)/sizeof(quadsOfE[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfE+i, DEPTH_OF_ALPHABETS, vaosE+i, vbosE+i); 
}  

void renderAlphabet_E(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosE, numQuadsInE); 
}  

void uninitializeAlphabet_E() 
{
    if(vbosE[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInE, vbosE); 
        memset((void*)vbosE, 0, sizeof(vbosE)); 
    } 

    if(vaosE[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInE, vaosE); 
        memset((void*)vaosE, 0, sizeof(vaosE)); 
    } 
} 

// --------- D ---------- 
const int numQuadsInD = 4; 
static struct AlphabetQuad quadsOfD[numQuadsInD] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {-0.4f, 1.0f, -0.7f, 1.0f, -0.7f, -1.0f, -0.4f, -1.0f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, 0.7f, 1.0f, 0.7f, -1.0f, 1.0f, -1.0f, alphabetColor}  
    }; 
GLuint vaosD[numQuadsInD]; 
GLuint vbosD[numQuadsInD];

void initAlphabet_D() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfD)/sizeof(quadsOfD[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfD+i, DEPTH_OF_ALPHABETS, vaosD+i, vbosD+i); 
}  

void renderAlphabet_D(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosD, numQuadsInD); 
}  

void uninitializeAlphabet_D() 
{
    if(vbosD[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInD, vbosD); 
        memset((void*)vbosD, 0, sizeof(vbosD)); 
    } 

    if(vaosD[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInD, vaosD); 
        memset((void*)vaosD, 0, sizeof(vaosD)); 
    } 
} 

// --------- I ---------- 
const int numQuadsInI = 3; 
static struct AlphabetQuad quadsOfI[numQuadsInI] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {0.15f, 1.0f, -0.15f, 1.0f, -0.15f, -1.0f, 0.15f, -1.0f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor}
    }; 
GLuint vaosI[numQuadsInI]; 
GLuint vbosI[numQuadsInI];

void initAlphabet_I() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfI)/sizeof(quadsOfI[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfI+i, DEPTH_OF_ALPHABETS, vaosI+i, vbosI+i); 
}  

void renderAlphabet_I(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosI, numQuadsInI); 
}  

void uninitializeAlphabet_I() 
{
    if(vbosI[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInI, vbosI); 
        memset((void*)vbosI, 0, sizeof(vbosI)); 
    } 

    if(vaosI[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInI, vaosI); 
        memset((void*)vaosI, 0, sizeof(vaosI)); 
    } 
} 

// --------- C ---------- 
const int numQuadsInC = 3; 
static struct AlphabetQuad quadsOfC[numQuadsInC] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, -0.7f, -1.0f, -0.7f, -1.0f, -1.0f, 1.0f, -1.0f, alphabetColor} 
    }; 
GLuint vaosC[numQuadsInC]; 
GLuint vbosC[numQuadsInC];

void initAlphabet_C() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfC)/sizeof(quadsOfC[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfC+i, DEPTH_OF_ALPHABETS, vaosC+i, vbosC+i); 
}  

void renderAlphabet_C(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosC, numQuadsInC); 
}  

void uninitializeAlphabet_C() 
{
    if(vbosC[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInC, vbosC); 
        memset((void*)vbosC, 0, sizeof(vbosC)); 
    } 

    if(vaosC[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInC, vaosC); 
        memset((void*)vaosC, 0, sizeof(vaosC)); 
    } 
} 

// --------- P ---------- 
const int numQuadsInP = 4; 
static struct AlphabetQuad quadsOfP[numQuadsInP] = {
    // p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, {r, g, b, a}
        {-0.7f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -0.7f, -1.0f, alphabetColor}, 
        {1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 0.7f, 1.0f, 0.7f, alphabetColor}, 
        {1.0f, 1.0f, 0.7f, 1.0f, 0.7f, 0.0f, 1.0f, 0.0f, alphabetColor}, 
        {1.0f, 0.15f, -1.0f, 0.15f, -1.0f, -0.15f, 1.0f, -0.15f, alphabetColor} 
    }; 
GLuint vaosP[numQuadsInP]; 
GLuint vbosP[numQuadsInP];

void initAlphabet_P() 
{
    // function declarations 
    void prepareCubeFromQuadVertices(AlphabetQuad* quad, float depth, GLuint* vao, GLuint* vbo); 

    // code     
    for(size_t i = 0; i < sizeof(quadsOfP)/sizeof(quadsOfP[0]); ++i) 
        prepareCubeFromQuadVertices(quadsOfP+i, DEPTH_OF_ALPHABETS, vaosP+i, vbosP+i); 
}  

void renderAlphabet_P(void) 
{
	// function declarations 
	void renderAplhabet(GLuint vaos[], int numQuads); 

	// code 
    renderAplhabet(vaosP, numQuadsInP); 
}  

void uninitializeAlphabet_P() 
{
    if(vbosP[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInP, vbosP); 
        memset((void*)vbosP, 0, sizeof(vbosP)); 
    } 

    if(vaosP[0] != 0) 
    {
        glDeleteVertexArrays(numQuadsInP, vaosP); 
        memset((void*)vaosP, 0, sizeof(vaosP)); 
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

void renderAplhabet(GLuint vaos[], int numQuads) 
{
	for(size_t i = 0; i < numQuads; ++i) 
    {
        glBindVertexArray(vaos[i]); 
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 4, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 8, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 12, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 16, 4); 
        glDrawArrays(GL_TRIANGLE_FAN, 20, 4); 
        glBindVertexArray(0); 
    } 
} 
