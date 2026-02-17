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

