#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif

int main(int argc, char *argv[])
{
	int isSuccess, looper;
	char *jsonContents;
	options mainOpt;
	SDL_Window *mainWin;
	SDL_Renderer *render;
	SDL_Event eventHandler;
	baseEntity *title, **buttons, **sprites, **dead;
	
	isSuccess = SUCCESS;
	jsonContents = loadJsonFile(OPTIONS_FILE,&isSuccess);
	mainOpt = initOptions(jsonContents, &isSuccess);
	dumpOptions(&mainOpt);
	mainWin = initSDL(&mainOpt, &isSuccess);
	render = createRenderer(mainWin, &isSuccess);
	
	title = initBaseEntity(mainOpt.title_img, render, &isSuccess, &mainOpt);
	while(isSuccess == SUCCESS)
	{
		while(SDL_PollEvent(&eventHandler) != 0)
		{
			if(eventHandler.type == SDL_QUIT)
			{
				isSuccess = FAIL;
			
			}
		
		
		}
		
		SDL_RenderClear(render);
		SDL_SetRenderDrawColor(render,mainOpt.R_COL,mainOpt.G_COL,mainOpt.B_COL,mainOpt.A_COL);
		SDL_RenderCopy(render,title->tex, NULL, &(title->dimensions));
		SDL_RenderPresent(render);
	
	}
	deinit();
	return 0;
}