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

void handleMenuButtons(entity **menuButtons, baseEntity *mouse, SDL_Event *events, int *inBattle, int *success)
{
	if(checkButtonClicked(mouse, menuButtons[START_BUT_MENU], events) == SUCCESS && *inBattle != SUCCESS)
	{			
		*inBattle = SUCCESS;
	}
	if(checkButtonClicked(mouse, menuButtons[QUIT_BUT_MENU], events) == SUCCESS & *success == SUCCESS)
	{
		*success = FAIL;
	}
	

}

void handleGameButtons(entity **gameButtons, entity **men, baseEntity **corpses, soldiers *army, baseEntity *mouse, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds, soldiers *bullets, entity *shell, entity *gas, Mix_Chunk **explosionSounds)
{

	if(checkButtonClicked(mouse, gameButtons[ANZACBUY], events) == SUCCESS)
	{
		newSquad(army,opt,men[ANZACSPR], corpses,success, deathsounds);
		
	}
	else if(checkButtonClicked(mouse, gameButtons[BEFBUY], events) == SUCCESS)
	{
		newSquad(army,opt,men[BEFSPR], corpses,success, deathsounds);
	
	}
	else if(checkButtonClicked(mouse, gameButtons[ARTILLERYBUY], events) == SUCCESS && *success == SUCCESS)
	{
		newShells(bullets, opt, shell, success, explosionSounds, BRITISH);
	}
	else if(checkButtonClicked(mouse, gameButtons[GASBUY], events) == SUCCESS && *success == SUCCESS)
	{
		newShells(bullets, opt, gas, success, explosionSounds, BRITISH);
	}
	else if(checkButtonClicked(mouse, gameButtons[AUDIO_ONBUT], events) == SUCCESS)
	{
		Mix_Volume(-1, MAX_VOL);
	
	}
	else if(checkButtonClicked(mouse, gameButtons[AUDIO_OFFBUT], events) == SUCCESS)
	{
		Mix_Volume(-1, MIN_VOL);
	
	}


}
void handleKeyboard(entity **men, baseEntity **corpses, soldiers *army, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds, soldiers *bullets, entity *shell, entity *gas, Mix_Chunk **explosionSounds)
{
	switch(events->key.keysym.sym)
	{
		case SDLK_1:
			newSquad(army,opt,men[LANDWEHRSPR], corpses,success, deathsounds);
			break;
		case SDLK_2:
			newSquad(army,opt,men[STURMTRUPPENSPR], corpses,success, deathsounds);
			break;
		case SDLK_3:
			newShells(bullets, opt, shell, success, explosionSounds, GERMAN);
			break;
		case SDLK_4:
			newShells(bullets, opt, gas, success, explosionSounds, GERMAN);
			break;
		case SDLK_ESCAPE:
			*success = FAIL;
			break;
		case SDLK_p:
			killAll(army);
			break;
		
	
	}





}
int checkScoreSide(soldiers *sold, int positionX,options *opt)
{
	int looper, score;
	score = 0;
	SDL_Rect hitBox = {positionX, 0, 1, opt->SCREEN_HEIGHT};
	for(looper = 0; looper < sold->no_men ; looper++)
	{
		
		if(SDL_HasIntersection(&(sold->men[looper]->posAndHitbox), &hitBox) == SDL_TRUE && sold->men[looper]->isAnimated == SUCCESS && sold->men[looper]->scored == FAIL)
		{
			score += -1;
			sold->men[looper]->scored = SUCCESS;
		
		}
	
	
	}


	return score;



}

void checkCollision(soldiers *bullets, soldiers *meatbags, Mix_Chunk **deaths, int *deadNo)
{
	int looperBullet, looperMeatbags, randomDeath ,noDead;
	noDead = 0;
	for(looperBullet = 0; looperBullet < bullets->no_men; looperBullet++)
	{
		for(looperMeatbags = 0; looperMeatbags < meatbags->no_men; looperMeatbags++)
		{
			if(SDL_HasIntersection(&(bullets->men[looperBullet]->posAndHitbox),&(meatbags->men[looperMeatbags]->posAndHitbox)) == SDL_TRUE && bullets->men[looperBullet]->isAnimated == SUCCESS &&  meatbags->men[looperMeatbags]->isAnimated == SUCCESS && bullets->men[looperBullet]->frame.x != 128)
			{
				randomDeath = rand() % 6;
				if(bullets->men[looperBullet]->type != 5)
				{
					bullets->men[looperBullet]->isAnimated = FAIL;
				}
				meatbags->men[looperMeatbags]->isAnimated = FAIL;
				Mix_PlayChannel(-1,deaths[randomDeath], 0);
				meatbags->men[looperMeatbags]->frame.x = 0;
				meatbags->men[looperMeatbags]->frame.y = 0;
				noDead++;
			}
		
		
		}
	
	
	}
	*deadNo = *deadNo + noDead;

}


