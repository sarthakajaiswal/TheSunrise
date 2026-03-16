#ifndef _OGL_HPP 
#define _OGL_HPP 

// ====================== HEADERS ===========================
#include <Windows.h> 

#include "global_header.hpp" 
#include "resources.hpp"
#include "fileHandler.hpp" 
#include "eventManager.hpp" 

// scene related headers 
#include "scenes/testScene.hpp" 
#include "scenes/introScene.hpp" 
#include "scenes/scene1.hpp" 
#include "scenes/scene2.hpp" 
#include "scenes/outroScene.hpp" 
#include "scenes/testScene.hpp" 

// ds 
#include "ds/stack.hpp" 

// imgui 
#include "imgui_manager.hpp" 

// ===================== LIBRARIES ==========================
#pragma comment(lib, "user32.lib") 
#pragma comment(lib, "gdi32.lib") 
#pragma comment(lib, "glew32.lib") 
#pragma comment(lib, "opengl32.lib") 
#pragma comment(lib, "OpenAL32.lib") 

// ===================== GLOBAL FUNCTIONS ===================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 

#endif /* _OGL_HPP */ 
