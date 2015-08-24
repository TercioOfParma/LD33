#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int isSuccess, inBattle, germanHP, britishHP;
	char *jsonContents, *soldierData;
	options mainOpt;
	SDL_Window *mainWin;
	SDL_Renderer *render;
	SDL_Event eventHandler;
	baseEntity *title, **buttons, **sprites, **dead, *map;
	soldiers *germans, *british, *bullets;
	unitData **allUnits;
	baseEntity mouse;
	entity *titleToDisp;
	entity **titleElements, **unitElements, **units;
	Mix_Chunk **deathSounds, **booms;
	
	isSuccess = SUCCESS;
	inBattle = FAIL;
	jsonContents = loadJsonFile(OPTIONS_FILE,&isSuccess);
	soldierData = loadJsonFile(SOLDIERS_FILE, &isSuccess);
	mainOpt = initOptions(jsonContents, &isSuccess);
	allUnits = loadUnitData(soldierData, &isSuccess, &mainOpt);
	mainWin = initSDL(&mainOpt, &isSuccess);
	render = createRenderer(mainWin, &isSuccess);
	title = initBaseEntity(mainOpt.title_img, render, &isSuccess, &mainOpt);
	titleToDisp = initTitle(*title, &isSuccess, &mainOpt);
	buttons = loadButtons(render,&isSuccess, &mainOpt); 
	sprites = loadSprites(render, &isSuccess, &mainOpt);
	dead = loadCorpses(render, &isSuccess, &mainOpt);
	titleElements = createMenuButtons(buttons, &isSuccess, &mainOpt);
	map = loadMap(mainOpt.map_path, render, &isSuccess, &mainOpt);  
	unitElements = createGameButtons(buttons, &isSuccess, &mainOpt);
	units = loadUnits(allUnits, sprites, &isSuccess, &mainOpt);
	deathSounds = loadDeathSounds(&isSuccess);
	booms = loadExplosions(&isSuccess);
	mouse.dimensions.w = 1;
	mouse.dimensions.h = 1;
	british = createArmy(allUnits[BRITMGSPR],BRITISH,sprites[BRITMGSPR], &isSuccess, &mainOpt);
	germans = createArmy(allUnits[GERMMGSPR],GERMAN,sprites[GERMMGSPR], &isSuccess, &mainOpt);
	bullets = createArmy(allUnits[GERMMGSPR], NO_SIDE, sprites[GERMMGSPR], &isSuccess, &mainOpt);
	germanHP = mainOpt.HP_PER_SIDE;
	britishHP = mainOpt.HP_PER_SIDE;
	while(isSuccess == SUCCESS)
	{
		SDL_GetMouseState(&(mouse.dimensions.x), &(mouse.dimensions.y));
		
		while(SDL_PollEvent(&eventHandler) != 0)
		{
			if(eventHandler.type == SDL_QUIT)
			{
				isSuccess = FAIL;
			
			}
			if(eventHandler.type == SDL_MOUSEBUTTONDOWN && inBattle != SUCCESS)
			{
				handleMenuButtons(titleElements, &mouse, &eventHandler, &inBattle, &isSuccess);
			
			}
			else if(eventHandler.type == SDL_MOUSEBUTTONDOWN && inBattle == SUCCESS)
			{
				
				handleGameButtons(unitElements, units, dead,  british, &mouse, &eventHandler, &isSuccess, &mainOpt, deathSounds);
				
				fprintf(stderr,"%d\n", british->no_men);
			}
			else if(eventHandler.type == SDL_KEYDOWN && inBattle == SUCCESS)
			{
				handleKeyboard(units, dead, germans, &eventHandler, &isSuccess, &mainOpt, deathSounds);
				
			
			}
		
		}
		SDL_RenderClear(render);
		SDL_SetRenderDrawColor(render,mainOpt.R_COL,mainOpt.G_COL,mainOpt.B_COL,mainOpt.A_COL);
		if(inBattle != SUCCESS)
		{
			SDL_RenderCopy(render,titleToDisp->liveAnimation, NULL, &(titleToDisp->posAndHitbox));
			drawMenuButtons(titleElements, render, &mainOpt);

		}
		
		if(inBattle == SUCCESS)
		{
			british->men[0]->angle = changeMachineGunAngle(british->men[0], germans, &mainOpt, &isSuccess,booms, bullets ,units[LEWIS_MG]);
			british->men[1]->angle = changeMachineGunAngle(british->men[1], germans, &mainOpt, &isSuccess,booms, bullets ,units[LEWIS_MG]);
			germans->men[0]->angle = changeMachineGunAngle(germans->men[0], british, &mainOpt, &isSuccess, booms, bullets, units[MAXIM_MG]);
			germans->men[1]->angle = changeMachineGunAngle(germans->men[1], british, &mainOpt, &isSuccess, booms, bullets, units[MAXIM_MG]);
			moveArmy(british);
			moveArmy(germans);
			moveArmy(bullets);
			checkCollision(germans, bullets, deathSounds);
			checkCollision(british, bullets, deathSounds);
			drawBaseEntity(map, render);
			drawGameButtons(unitElements, render, &mainOpt);
			drawArmy(british, render, &mainOpt);
			drawArmy(germans, render, &mainOpt);
			drawArmy(bullets, render, &mainOpt);
			germanHP += checkScoreSide(british,0);
			britishHP += checkScoreSide(germans, mainOpt.SCREEN_WIDTH);
			if(britishHP < 0 || germanHP < 0)
			{
				isSuccess = FAIL;
			
			}
			
		}
		
		SDL_RenderPresent(render);
	}
	freeEntityArray(british->men, british->no_men);
	freeEntityArray(germans->men, germans->no_men);
	freeEntityArray(bullets->men, bullets->no_men);
	freeEntityArray(titleElements, 2);
	freeEntityArray(unitElements, 6);
	freeButtons(buttons, &mainOpt);
	freeSprites(sprites, &mainOpt);
	freeCorpses(dead, &mainOpt);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(mainWin);
	deinit();
	return 0;
}