#pragma once 

#include "global_header.hpp" 

enum Scene 
{
    INTRO_SCENE = 0, 
    SCENE_1, 
    SCENE_2, 
    TEST_SCENE 
}; 

enum Shot 
{
    SHOT1, 
    SHOT2, 
    SHOT3,  
    SHOT4,  
    SHOT5,  
    SHOT6,  
    SHOT7,  
}; 

extern const float introSceneStartTime; 

void updateEvent(void); 
