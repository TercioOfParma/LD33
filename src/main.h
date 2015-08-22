#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <jansson/jansson.h>
#include <stdio.h>
#include <stdlib.h>


const static int SUCCESS = 1;
const static int FAIL = 2;
const static char *OPTIONS_FILE = "options.json";
const static int MAX_LINE = 80;

typedef enum
{
	ANZACBUY,
	BEFBUY,
	ARTILLERYBUY,
	GASBUY,
	AUDIO_ONBUT,
	AUDIO_OFFBUT,
	STARTBUT,
	QUITBUT


}buttonPos;
typedef enum
{
	BEF,
	ANZAC,
	LANDWEHR,
	STURMTRUPPEN,
	TITLE,
	START,
	QUIT,
	BUYFAST,//sturmtruppen / anzac
	BUYCHEAP // BEF / Landwehr
	


}unitType;
typedef enum
{
	ANZACSPR,
	BEFSPR,
	ARTILLERYSPR,
	LANDWEHRSPR,
	STURMTRUPPENSPR,
	GASSPR,
	BRITMGSPR,
	GERMMGSPR

}spritePos;
typedef enum
{
	NO_SIDE,
	GERMAN,
	BRITISH



}sideData;
typedef struct
{
	int SCREEN_WIDTH, SCREEN_HEIGHT, SAMPLE_FREQUENCY, NO_CHANNELS, SAMPLE_SIZE,R_COL, G_COL, B_COL, A_COL, NO_BUTTONS, NO_CORPSES, NO_SPRITES, QUIT_OFFSET;
	const char *title_img, *start_button, *quit_button, *title, *sprite_path, *corpses_path, *buttons_path, *map_path;
	double SCALE_FACTOR;
}options;
typedef struct
{
	int type, speed, side;
	char isAnimated;
	double angle;
	SDL_Texture *liveAnimation, *deadAnimation;//these will simply point to base textures
	SDL_Rect posAndHitbox;
	
}entity;

typedef struct//this is so you can load a element and then copy it to an entity
{
	SDL_Texture *tex;
	SDL_Rect dimensions;

}baseEntity;
typedef struct
{
	entity **men;
	int no_men;


}soldiers;

//init functions
char *loadJsonFile(const char *filename, int *success);
options initOptions(const char *jsonFile, int *success);
SDL_Window *initSDL(options *opt, int *success);
int getFileSize(FILE *sizeToGet, int *success);
void deinit();
void dumpOptions(options *opt);
SDL_Renderer *createRenderer(SDL_Window *screen, int *success);
TTF_Font *LoadFont(const char *filename, int size, int *success);
Mix_Chunk *loadEffect(const char *filename, int *success);
Mix_Music *loadMusic(const char *filename, int *success);
baseEntity *initBaseEntity(const char *filename, SDL_Renderer *render, int *success, options *opt);
entity *initEntity(int desiredType, int side, baseEntity liveAnimation, baseEntity deadAnimation, int *success, options *opt);
entity *initTitle(baseEntity animation, int *success, options *opt);
baseEntity **loadButtons(SDL_Renderer *render, int *success, options *opt);
baseEntity **loadSprites(SDL_Renderer *render, int *success, options *opt);
baseEntity **loadCorpses(SDL_Renderer *render, int *success, options *opt);
void freeButtons(baseEntity **buttons, options *opt);
void freeSprites(baseEntity **sprites, options *opt);
void freeCorpses(baseEntity **corpses, options *opt);
entity **createMenuButtons(baseEntity **buttons, int *success, options *opt);
baseEntity *loadMap(const char *filename, SDL_Renderer *render, int *success, options *opt);


//draw functions
void drawMenuButtons(entity **menuButtons, SDL_Renderer *render);
void drawEntity(entity *toDraw, SDL_Renderer *render);
void drawBaseEntity(baseEntity *toDraw, SDL_Renderer *render);

//inputFunctions
int checkButtonClicked(baseEntity *mouse, entity *startButton, SDL_Event *events);