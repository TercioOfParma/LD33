#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



void drawEntity(entity *toDraw, SDL_Renderer *render, options *opt)
{
	
	if(toDraw->posAndHitbox.x >= 0 && toDraw->posAndHitbox.x < opt->SCREEN_WIDTH && toDraw->posAndHitbox.y >= 0 && toDraw->posAndHitbox.y < opt->SCREEN_HEIGHT)
	{
		if(toDraw->anim == opt->FRAMES_PER_ANIM && toDraw->isAnimated == SUCCESS)
		{
			if(toDraw->frame.x == 0)
			{
				toDraw->frame.x = 64;
				toDraw->frame.y = 0;
			
			}
			else
			{
				toDraw->frame.x = 0;
				toDraw->frame.y = 0;
			
			}
			toDraw->anim = 0;
		}
		if(toDraw->isAnimated == SUCCESS && toDraw->frame.x != INVALID_RECT)
		{
		
			SDL_RenderCopyEx(render, toDraw->liveAnimation, &(toDraw->frame), &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
			toDraw->anim++;
		}
		else if(toDraw->isAnimated == SUCCESS)
		{
	
			SDL_RenderCopyEx(render, toDraw->liveAnimation, NULL, &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
	
		}
		else
		{
			SDL_RenderCopyEx(render, toDraw->deadAnimation, NULL, &(toDraw->posAndHitbox), toDraw->angle, NULL, SDL_FLIP_NONE);
			
		}
		
		
	}

}

void drawMenuButtons(entity **menuButtons, SDL_Renderer *render, options *opt)
{
	drawEntity(menuButtons[0], render, opt);
	drawEntity(menuButtons[1], render, opt);



}
void drawGameButtons(entity **gameButtons, SDL_Renderer *render, options *opt)
{
	int looper;
	for(looper = 0; looper < opt->NO_BUTTONS - 2; looper++)
	{

		drawEntity(gameButtons[looper], render, opt);
	
	}



}

void drawBaseEntity(baseEntity *toDraw, SDL_Renderer *render)
{	
	SDL_RenderCopy(render, toDraw->tex, NULL, &(toDraw->dimensions));
	
}

void drawArmy(soldiers *toDraw, SDL_Renderer *render, options *opt)
{
	int looper;
	for(looper = 0; looper < toDraw->no_men; looper++)
	{
		drawEntity(toDraw->men[looper], render, opt);
	
	}


}

void moveArmy(soldiers *toMove)
{
	int looper, op, adj;
	for(looper = 0; looper < toMove->no_men; looper++)
	{
		if(toMove->men[looper]->isAnimated == SUCCESS)
		{
			if(toMove->men[looper]->side == GERMAN)
			{
				toMove->men[looper]->posAndHitbox.x += toMove->men[looper]->speed;
				
			}
			else if(toMove->men[looper]->side == BRITISH)
			{
				toMove->men[looper]->posAndHitbox.x -= toMove->men[looper]->speed;
				
			}
			else if(toMove->men[looper]->side == NO_SIDE)
			{
				toMove->men[looper]->posAndHitbox.x += toMove->men[looper]->adj;
				toMove->men[looper]->posAndHitbox.y += toMove->men[looper]->op;
			}
		
		}
	
	
	}



}

double changeMachineGunAngle(entity *MG, soldiers *opposingArmy, options *opt, int *success, Mix_Chunk **sounds, soldiers *bullets, entity *bullet)
{
	int looper, diffX, diffY, smallX, smallY,op, adj,quadrant;
	double angle = MG->angle;
	smallX = INVALID_RECT; //nice large number
	smallY = INVALID_RECT; //nice large number
	for(looper = 0; looper < opposingArmy->no_men; looper++)
	{
		if(opposingArmy->men[looper]->isAnimated == SUCCESS &&  opposingArmy->men[looper]->posAndHitbox.x > 0 &&  opposingArmy->men[looper]->posAndHitbox.x < opt->SCREEN_WIDTH && opposingArmy->men[looper]->posAndHitbox.y > 0 &&  opposingArmy->men[looper]->posAndHitbox.y < opt->SCREEN_HEIGHT)
		{
			diffX = opposingArmy->men[looper]->posAndHitbox.x;
			diffY = opposingArmy->men[looper]->posAndHitbox.y;
			if(sqrt((diffX * diffX) + (diffY * diffY)) < sqrt((smallX * smallX) + (smallY * smallY)))
			{
				smallX = diffX;
				smallY = diffY;
			
			}
		}
	}
	if(smallX == INVALID_RECT || smallY == INVALID_RECT)
	{
		return angle;
	}
	smallX -= MG->posAndHitbox.x + (rand() % opt->ACCURACY_DEVIATION - (opt->ACCURACY_DEVIATION / 2));
	smallY -= MG->posAndHitbox.y + (rand() % opt->ACCURACY_DEVIATION - (opt->ACCURACY_DEVIATION / 2));
	angle = ((atan2(smallY, smallX)* PI_DEG) / PI);
	quadrant = ((int)angle/PI_DEG * 2) % 4 + 1;
	
	if(abs(sqrt((smallX * smallX) + (smallY * smallY))) < opt->MG_RANGE )
	{
		
		op = bullet->speed * sin((angle * PI) / PI_DEG);
		adj = bullet->speed * cos((angle * PI) / PI_DEG);
		newObject(bullets, opt, bullet,success, sounds, 1, MG, op, adj);
	
		
	}
	
	
	return angle;
}