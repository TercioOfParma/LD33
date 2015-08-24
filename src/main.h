#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <jansson/jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


const static int SUCCESS = 1;
const static int FAIL = 2;
const static char *OPTIONS_FILE = "options.json";
const static int MAX_LINE = 80;
const static int START_BUT_MENU = 0;
const static int QUIT_BUT_MENU = 1;
const static char *NO_SOUND = "NA";
const static char *SOLDIERS_FILE = "soldiers.json";
const static int TRUE = 1;
const static int FALSE = 0;
const static int INVALID_RECT = 123456;
const static double PI = 3.141592653;
const static int MAX_VOL = 128;
const static int MIN_VOL = 0;
const static int PI_DEG = 180;
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
	BUYCHEAP, // BEF / Landwehr
	LEWIS_BULLET,
	MAXIM_BULLET


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
	GERMMGSPR,
	LEWIS_MG,
	MAXIM_MG

}spritePos;
typedef enum
{
	NO_SIDE,
	BRITISH,
	GERMAN
}sideData;
typedef enum
{
	DEATH_SOUND_1,
	DEATH_SOUND_2,
	DEATH_SOUND_3,
	DEATH_SOUND_4,
	DEATH_SOUND_5,
	DEATH_SOUND_6,
}audioData;

typedef struct
{
	int SCREEN_WIDTH, SCREEN_HEIGHT, SAMPLE_FREQUENCY, NO_CHANNELS, SAMPLE_SIZE,R_COL, G_COL, B_COL, A_COL, NO_BUTTONS, NO_CORPSES, NO_SPRITES, QUIT_OFFSET, BUTTON_TRANSPARENCY, NO_UNITS, NO_SOUNDS, SQUAD_SIZE, HP_PER_SIDE;
	int ROF, ACCURACY_DEVIATION, OTHER_OFFSET, MG_RANGE, FRAMES_PER_ANIM;
	const char *title_img, *start_button, *quit_button, *title, *sprite_path, *corpses_path, *buttons_path, *map_path;
	double SCALE_FACTOR;
}options;
typedef struct
{
	int type, speed, side, cost, op, adj, anim, shotTime, endTime, startTime;
	char isAnimated;
	double angle;
	SDL_Texture *liveAnimation, *deadAnimation;//these will simply point to base textures
	SDL_Rect posAndHitbox, frame;
	Mix_Chunk *entranceSound, *deathSound;
	
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
typedef struct
{
	int ID,side, cost, speed;
	const char *entrance_filename;


}unitData;

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
entity **createMenuButtons(baseEntity **buttons, int *success, options *opt);
baseEntity *loadMap(const char *filename, SDL_Renderer *render, int *success, options *opt);
entity **createGameButtons(baseEntity **buttons, int *success, options *opt);
entity *loadMachineGun(unitData *mGData, int side, baseEntity *mGTex, int *success, options *opt);
soldiers *createArmy(unitData *mGData, int side, baseEntity *mGText, int *success, options *opt);
unitData **loadUnitData( const char *file, int *success, options *opt);
entity **loadUnits(unitData **data, baseEntity **textures, int *success, options *opt);
void newSquad(soldiers *army, options *opt, entity *unitType, baseEntity **corpses, int *success, Mix_Chunk **deathsounds);
Mix_Chunk **loadDeathSounds(int *success);
void killAll(soldiers *army);
void newObject(soldiers *army, options *opt, entity *unitType,int *success, Mix_Chunk **sounds, int number, entity *mg, int op, int adj);
Mix_Chunk **loadExplosions(int *success);

//deinit functions
void freeEntityArray(entity **array, int size);
void freeButtons(baseEntity **buttons, options *opt);
void freeSprites(baseEntity **sprites, options *opt);
void freeCorpses(baseEntity **corpses, options *opt);

//draw functions
void drawMenuButtons(entity **menuButtons, SDL_Renderer *render, options *opt);
void drawEntity(entity *toDraw, SDL_Renderer *render, options *opt);
void drawBaseEntity(baseEntity *toDraw, SDL_Renderer *render);
void drawGameButtons(entity **gameButtons, SDL_Renderer *render, options *opt);
void drawArmy(soldiers *toDraw, SDL_Renderer *render, options *opt);
void moveArmy(soldiers *toMove);
double changeMachineGunAngle(entity *MG, soldiers *opposingArmy, options *opt, int *success, Mix_Chunk **sounds, soldiers *bullets, entity *bullet);
void checkCollision(soldiers *bullets, soldiers *meatbags, Mix_Chunk **deaths);

//input Functions
int checkButtonClicked(baseEntity *mouse, entity *startButton, SDL_Event *events);
void handleMenuButtons(entity **menuButtons, baseEntity *mouse, SDL_Event *events, int *inBattle, int *success);
void handleGameButtons(entity **gameButtons, entity **men, baseEntity **corpses, soldiers *army, baseEntity *mouse, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds);
void handleKeyboard(entity **men, baseEntity **corpses, soldiers *army, SDL_Event *events, int *success, options *opt, Mix_Chunk **deathsounds);
int checkScoreSide(soldiers *sold, int positionX);