#pragma once 

#include "global_header.hpp" 

enum Scene 
{
    INTRO_SCENE = 0, 
    SCENE_1, 
    SCENE_2, 
    SCENE_3 
}; 

extern const float introSceneStartTime; 

void updateEvent(void); 
