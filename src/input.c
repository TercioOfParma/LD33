#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



int checkButtonClicked(baseEntity *mouse, entity *startButton, SDL_Event *events)
{
	if(SDL_HasIntersection(&(mouse->dimensions), &(startButton->posAndHitbox)) == SDL_TRUE)
	{	
		
		return SUCCESS;
	
	}

	else
	{
	
		return FAIL;
	}

}
