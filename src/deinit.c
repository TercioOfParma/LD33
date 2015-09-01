#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif






void freeButtons(baseEntity **buttons, options *opt)
{
	int looper;

	for(looper = 0; looper < opt->NO_BUTTONS; looper++)
	{
		SDL_DestroyTexture(buttons[looper]->tex);
	}
	free(*buttons);
	free(buttons);

}
void freeSprites(baseEntity **sprites, options *opt)
{
	int looper;

	for(looper = 0; looper < opt->NO_SPRITES; looper++)
	{
		SDL_DestroyTexture(sprites[looper]->tex);
	}
	free(*sprites);
	free(sprites);

}
void freeCorpses(baseEntity **corpses, options *opt)
{
	int looper;

	for(looper = 0; looper < opt->NO_CORPSES; looper++)
	{
		SDL_DestroyTexture(corpses[looper]->tex);
	}
	free(*corpses);
	free(corpses);

}
void freeEntityArray(entity **array, int size)
{
	int looper;

	for(looper = 0; looper < size; looper++)
	{
		SDL_DestroyTexture(array[looper]->liveAnimation);
		SDL_DestroyTexture(array[looper]->deadAnimation);
	}
	free(*array);
	free(array);


}
void freeChunks(Mix_Chunk **chunkArray, int size)
{
	int looper;
	
	for(looper = 0; looper < size; looper++)
	{
		Mix_FreeChunk(chunkArray[looper]);
	
	}


}