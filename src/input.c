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

void handleGameButtons(entity **gameButtons, entity **men, baseEntity **corpses, soldiers *army, baseEntity *mouse, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds)
{

	if(checkButtonClicked(mouse, gameButtons[ANZACBUY], events) == SUCCESS)
	{
		newSquad(army,opt,men[ANZACSPR], corpses,success, deathsounds);
		
	}
	else if(checkButtonClicked(mouse, gameButtons[BEFBUY], events) == SUCCESS)
	{
		newSquad(army,opt,men[BEFSPR], corpses,success, deathsounds);
	
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
void handleKeyboard(entity **men, baseEntity **corpses, soldiers *army, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds)
{
	switch(events->key.keysym.sym)
	{
		case SDLK_1:
			newSquad(army,opt,men[LANDWEHRSPR], corpses,success, deathsounds);
			break;
		case SDLK_2:
			newSquad(army,opt,men[STURMTRUPPENSPR], corpses,success, deathsounds);
			break;
		case SDLK_ESCAPE:
			*success = FAIL;
			break;
		case SDLK_p:
			killAll(army);
			break;
	
	}





}
int checkScoreSide(soldiers *sold, int positionX)
{
	int looper, score;
	score = 0;
	for(looper = 0; looper < sold->no_men ; looper++)
	{
		if(sold->men[looper]->posAndHitbox.x == positionX && sold->men[looper]->isAnimated)
		{
			score += -1;
		
		}
	
	
	}


	return score;



}




