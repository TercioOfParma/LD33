#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



void drawEntity(entity *toDraw, SDL_Renderer *render)
{
	if(toDraw->isAnimated == SUCCESS)
	{
		SDL_RenderCopyEx(render, toDraw->liveAnimation, NULL, &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	}
	else
	{
		SDL_RenderCopyEx(render, toDraw->deadAnimation, NULL, &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	
	}

}

void drawMenuButtons(entity **menuButtons, SDL_Renderer *render)
{
	drawEntity(menuButtons[0], render);
	drawEntity(menuButtons[1], render);



}

void drawBaseEntity(baseEntity *toDraw, SDL_Renderer *render)
{	
	SDL_RenderCopy(render, toDraw->tex, NULL, &(toDraw->dimensions));
	
}