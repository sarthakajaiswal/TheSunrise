#include "../headers/eventManager.hpp" 

const float introSceneStartTime = 3.0f; 
const float introSceneEndTime = 20.0f; 

void updateEvent(void) 
{
    if(mainTimer >= introSceneStartTime && mainTimer < introSceneEndTime) 
        CurrentScene = INTRO_SCENE; 
} 
