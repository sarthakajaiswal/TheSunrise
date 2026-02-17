#ifndef _GLOBAL_HEADER_HPP 
#define _GLOBAL_HEADER_HPP 

#include <Windows.h> 

#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 

#include <gl/glew.h> 
#include <gl/GL.h> 

#include "vmath.h" 
using namespace vmath; 

#include "fileHandler.hpp" 

enum
{
    AMC_ATTRIBUTE_POSITION = 0, 
    AMC_ATTRIBUTE_COLOR, 
    AMC_ATTRIBUTE_TEXCOORD, 
    AMC_ATTRIBUTE_NORMAL, 
    AMC_ATTRIBUTE_TANGENT, 
}; 

// log file 
extern FileHandler logFile; 

// matrices 
extern mat4 projectionMatrix; 
extern mat4 viewMatrix; 

// window dimentions 
extern int winWidth, winHeight; 

#endif /* _GLOBAL_HEADER_HPP */ 
