#pragma once 

#include <string.h> 

#include <gl\glew.h> 
#include <gl\GL.h> 

#include "global_header.hpp" 

struct Color 
{
    float r, g, b, a; 
}; 
struct AlphabetQuad 
{
    // co-ordinates of vertices of quad 
    // starting from right-top to right-bottom in anticlockwise direction 
    float p1x, p1y; 
    float p2x, p2y; 
    float p3x, p3y; 
    float p4x, p4y; 

    struct Color color; 
}; 

void initAlphabet_A(); 
void renderAlphabet_A(); 
void uninitializeAlphabet_A(); 

void initAlphabet_S(void); 
void renderAlphabet_S(); 
void uninitializeAlphabet_S(); 

void initAlphabet_T(void); 
void renderAlphabet_T(); 
void uninitializeAlphabet_T(); 

void initAlphabet_R(void); 
void renderAlphabet_R(); 
void uninitializeAlphabet_R(); 

void initAlphabet_O(void); 
void renderAlphabet_O(); 
void uninitializeAlphabet_O(); 

void initAlphabet_M(void); 
void renderAlphabet_M(); 
void uninitializeAlphabet_M(); 

void initAlphabet_E(void); 
void renderAlphabet_E(); 
void uninitializeAlphabet_E(); 

void initAlphabet_I(void); 
void renderAlphabet_I(); 
void uninitializeAlphabet_I(); 

void initAlphabet_D(void); 
void renderAlphabet_D(); 
void uninitializeAlphabet_D(); 

void initAlphabet_C(void); 
void renderAlphabet_C(); 
void uninitializeAlphabet_C(); 

void initAlphabet_P(void); 
void renderAlphabet_P(); 
void uninitializeAlphabet_P(); 


