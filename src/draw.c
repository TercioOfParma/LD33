#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



void drawEntity(entity *toDraw, SDL_Renderer *render)
{
	if(toDraw->isAnimated == SUCCESS && toDraw->frame.x != INVALID_RECT)
	{
		
		SDL_RenderCopyEx(render, toDraw->liveAnimation, &(toDraw->frame), &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	}
	else if(toDraw->frame.x != INVALID_RECT)
	{
		SDL_RenderCopyEx(render, toDraw->deadAnimation, &(toDraw->frame), &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	
	}
	else if(toDraw->isAnimated == SUCCESS)
	{
	
		SDL_RenderCopyEx(render, toDraw->liveAnimation, NULL, &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	
	}
	else
	{
		SDL_RenderCopyEx(render, toDraw->deadAnimation, &(toDraw->frame), &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	}
}

void drawMenuButtons(entity **menuButtons, SDL_Renderer *render)
{
	drawEntity(menuButtons[0], render);
	drawEntity(menuButtons[1], render);



}
void drawGameButtons(entity **gameButtons, SDL_Renderer *render, options *opt)
{
	int looper;
	for(looper = 0; looper < opt->NO_BUTTONS - 2; looper++)
	{

		drawEntity(gameButtons[looper], render);
	
	}



}

void drawBaseEntity(baseEntity *toDraw, SDL_Renderer *render)
{	
	SDL_RenderCopy(render, toDraw->tex, NULL, &(toDraw->dimensions));
	
}

void drawArmy(soldiers *toDraw, SDL_Renderer *render)
{
	int looper;
	for(looper = 0; looper < toDraw->no_men; looper++)
	{
		drawEntity(toDraw->men[looper], render);
	
	}


}

void moveArmy(soldiers *toMove)
{
	int looper;
	for(looper = 0; looper < toMove->no_men; looper++)
	{
		if(toMove->men[looper]->isAnimated == SUCCESS)
		{
			if(toMove->men[looper]->side == GERMAN)
			{
				toMove->men[looper]->posAndHitbox.x += toMove->men[looper]->speed;
				
			}
			if(toMove->men[looper]->side == BRITISH)
			{
				toMove->men[looper]->posAndHitbox.x -= toMove->men[looper]->speed;
				
			}
		
		
		}
	
	
	}



}