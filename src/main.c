#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif

int main(int argc, char *argv[])
{
	srand(time(NULL));
	int isSuccess, inBattle, germanHP, britishHP, oldGermanHP,oldBritishHP, noDead, oldNoDead, germanPoints, britishPoints, oldGermanPoints, oldBritishPoints;
	char *jsonContents, *soldierData;
	options mainOpt;
	SDL_Window *mainWin;
	SDL_Renderer *render;
	SDL_Event eventHandler;
	SDL_Texture *germanHpDis, *britishHpDis, *deadDis, *germanPointDis, *britishPointDis;
	SDL_Rect germanRect, britishRect, deadRect, germanPRect, britishPRect;
	
	baseEntity *title, **buttons, **sprites, **dead, *map;
	soldiers *germans, *british, *bullets;
	unitData **allUnits;
	baseEntity mouse;
	entity *titleToDisp;
	entity **titleElements, **unitElements, **units;
	Mix_Chunk **deathSounds, **booms;
	TTF_Font *defaultFont;
	
	noDead = 0;
	oldNoDead = 0;
	
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
	defaultFont = loadFont(FONT_PATH, 20, render);
	mouse.dimensions.w = 1;
	mouse.dimensions.h = 1;
	british = createArmy(allUnits[BRITMGSPR],BRITISH,sprites[BRITMGSPR], &isSuccess, &mainOpt);
	germans = createArmy(allUnits[GERMMGSPR],GERMAN,sprites[GERMMGSPR], &isSuccess, &mainOpt);
	bullets = createArmy(allUnits[GERMMGSPR], NO_SIDE, sprites[GERMMGSPR], &isSuccess, &mainOpt);
	germanHP = mainOpt.HP_PER_SIDE;
	britishHP = mainOpt.HP_PER_SIDE;
	germanRect.y = mainOpt.SCREEN_HEIGHT - mainOpt.OTHER_OFFSET ; 
	germanRect.x = 0;
	germanPRect.x = 0;
	germanPRect.y = mainOpt.SCREEN_HEIGHT - mainOpt.OTHER_OFFSET + 70 ; 
	britishRect.y = mainOpt.SCREEN_HEIGHT - mainOpt.OTHER_OFFSET; 
	britishRect.x = mainOpt.SCREEN_WIDTH - (mainOpt.OTHER_OFFSET *1.5) - 100;
	britishPRect.x = mainOpt.SCREEN_WIDTH - (mainOpt.OTHER_OFFSET *1.5) - 70;
	britishPRect.y = mainOpt.SCREEN_HEIGHT - mainOpt.OTHER_OFFSET + 70;
	deadRect.x = (int)(mainOpt.SCREEN_WIDTH / 2);
	deadRect.y = 0;
	britishPoints = mainOpt.STARTING_POINTS;
	germanPoints = britishPoints;
	britishHpDis = renderScore(defaultFont, &britishRect, render , britishHP , britishHpDis, "British HP ");
	germanHpDis =  renderScore(defaultFont, &germanRect, render , germanHP , germanHpDis, "German HP ");
	germanPointDis =  renderScore(defaultFont, &germanPRect, render , germanPoints ,germanPointDis, "German Points ");
	britishPointDis =  renderScore(defaultFont, &britishPRect, render , britishPoints , britishPointDis, "British Points ");
	deadDis = renderScore(defaultFont, &deadRect, render , noDead , deadDis, "Dead men ");
	while(isSuccess != FAIL)
	{
		SDL_GetMouseState(&(mouse.dimensions.x), &(mouse.dimensions.y));
		oldBritishHP = britishHP;
		oldBritishPoints = britishPoints;
		oldGermanHP = germanHP;
		oldGermanPoints = germanPoints;
		oldNoDead = noDead;
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
				
				handleGameButtons(unitElements, units, dead,  british, &mouse, &eventHandler, &isSuccess, &mainOpt, deathSounds, bullets, units[ARTILLERYSPR] , units[GASSPR], booms, &britishPoints);
				fprintf(stderr, "%d\n", bullets->no_men);
			
			}
			else if(eventHandler.type == SDL_KEYDOWN && inBattle == SUCCESS)
			{
				handleKeyboard(units, dead, germans, &eventHandler, &isSuccess, &mainOpt, deathSounds, bullets, units[ARTILLERYSPR], units[GASSPR], booms, &germanPoints);
				
			
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
			checkCollision(bullets, british, deathSounds, &noDead);
			checkCollision(bullets, germans, deathSounds, &noDead);
			drawBaseEntity(map, render);
			drawArmy(british, render, &mainOpt);
			drawArmy(germans, render, &mainOpt);
			drawArmy(bullets, render, &mainOpt);
			drawGameButtons(unitElements, render, &mainOpt);
			germanHP += checkScoreSide(british,0, &mainOpt);
			britishHP += checkScoreSide(germans, mainOpt.SCREEN_WIDTH, &mainOpt);
			britishPoints++;
			germanPoints++;
			if(britishHP < 0 || germanHP < 0)
			{
				isSuccess = GAME_WON;
			
			}
			if(noDead != oldNoDead)
			{
				deadDis = renderScore(defaultFont, &deadRect, render , noDead , deadDis, "Dead men");
			}
			if(germanHP != oldGermanHP)
			{
				germanHpDis = renderScore(defaultFont, &germanRect, render , germanHP , germanHpDis, "German HP");
			
			}
			if(germanPoints != oldGermanPoints)
			{
				germanPointDis = renderScore(defaultFont, &germanPRect, render , germanPoints , germanPointDis, "German Points");
			
			}
			if(britishHP != oldBritishHP)
			{
				britishHpDis = renderScore(defaultFont, &britishRect, render , britishHP , britishHpDis, "British HP");
			
			}
			if(britishPoints != oldBritishPoints)
			{
				britishPointDis = renderScore(defaultFont, &britishRect, render , britishPoints , britishPointDis, "British Points");
			
			}
		
			
	
			
		}
		if(isSuccess == GAME_WON)
			{
				while(isSuccess != FAIL)
				{
					if(germanHP > 0)
					{
						deadDis = renderScore(defaultFont, &deadRect, render , noDead , deadDis, "GERMANY WON ");
						
					}
					else
					{
						deadDis = renderScore(defaultFont, &deadRect, render , noDead , deadDis, "BRITAIN WON ");
					}
							SDL_RenderClear(render);
							SDL_RenderCopy(render, deadDis, NULL, &deadRect);
							SDL_RenderPresent(render);
							while(SDL_PollEvent(&eventHandler) != 0)
							{
								if(eventHandler.type == SDL_KEYDOWN)
								{
										isSuccess = FAIL;
										goto end; //i am sorry dijkstra
								}	
							}	
				}
			}
		SDL_RenderCopy(render, britishHpDis, NULL, &britishRect);
		SDL_RenderCopy(render, germanHpDis, NULL, &germanRect);
		SDL_RenderCopy(render, deadDis, NULL, &deadRect);
		SDL_RenderCopy(render, britishPointDis, NULL, &britishPRect);
		SDL_RenderCopy(render, germanPointDis, NULL, &germanPRect);
		SDL_RenderPresent(render);
	}
	end://"let the OS deal with the others"
	freeEntityArray(british->men, british->no_men);
	freeEntityArray(germans->men, germans->no_men);
	freeEntityArray(bullets->men, bullets->no_men);
	freeEntityArray(titleElements, 2);
	freeEntityArray(unitElements, 6);
	freeButtons(buttons, &mainOpt);
	freeSprites(sprites, &mainOpt);
	freeCorpses(dead, &mainOpt);
	freeChunks(deathSounds, 6);
	freeChunks(booms,3);
	TTF_CloseFont(defaultFont);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(mainWin);
	deinit();
	return 0;
}