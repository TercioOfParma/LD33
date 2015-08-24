#ifndef INCLUDE_LOCK
#define INCLUDE_LOCK
#include "main.h"
#endif



char *loadJsonFile(const char *filename, int *success)
{
	FILE *jsonFile = fopen(filename, "rb");
	int fileSize, result, looper;
	char *fileContents, *truncatedContents;
	if(!jsonFile)
	{
		fprintf(stderr, "fopen has failed : %s \n", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	fileSize = getFileSize(jsonFile, success);
	if(!fileSize)
	{
		return NULL;
	
	}
	fileContents = malloc(sizeof(char) * fileSize);
	if(!fileContents)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	fread(fileContents,1,fileSize, jsonFile);
	fileContents[fileSize] = '\0';
	return fileContents;
}
int getFileSize(FILE *sizeToGet, int *success)
{
	int fileSize = 0;
	fseek(sizeToGet,0,SEEK_END);
	fileSize = ftell(sizeToGet);
	rewind(sizeToGet);
	if(errno > 0)
	{
		fprintf(stderr, "ftell has failed : %s", strerror(errno));
		*success = FAIL;
		return 0;
	}
	return fileSize;

}
options initOptions(const char *jsonFile, int *success)
{
	
	options tempOpt;
	json_t *tempJsonHandle, *optionsData;
	json_error_t errorHandle;
	
	tempJsonHandle = json_loads(jsonFile,0, &errorHandle);
	if(!tempJsonHandle)
	{
		fprintf(stderr, "json_loads has failed : %s \n", errorHandle.text);
		*success = FAIL;
		return tempOpt;
	
	}
	
	optionsData = json_array_get(tempJsonHandle, 0);
	if(!json_is_object(optionsData))
	{
		fprintf(stderr,"json_object_get failed, didn't get an object\n");
		*success = FAIL;
		json_decref(tempJsonHandle);
		return tempOpt;
	
	}
	
	tempOpt.SCREEN_WIDTH = json_integer_value(json_object_get(optionsData,"SCREEN_WIDTH"));
	tempOpt.SCREEN_HEIGHT = json_integer_value(json_object_get(optionsData,"SCREEN_HEIGHT"));
	tempOpt.title_img = json_string_value(json_object_get(optionsData,"TITLE_IMG"));
	tempOpt.start_button = json_string_value(json_object_get(optionsData,"START_IMG"));
	tempOpt.quit_button = json_string_value(json_object_get(optionsData,"QUIT_IMG"));
	tempOpt.corpses_path = json_string_value(json_object_get(optionsData,"CORPSE_PATH"));
	tempOpt.sprite_path = json_string_value(json_object_get(optionsData,"SPRITE_PATH"));
	tempOpt.buttons_path = json_string_value(json_object_get(optionsData,"BUTTON_PATH"));
	tempOpt.map_path = json_string_value(json_object_get(optionsData,"MAP_PATH"));
	tempOpt.title = json_string_value(json_object_get(optionsData, "TITLE"));
	tempOpt.SAMPLE_SIZE = json_integer_value(json_object_get(optionsData,"SAMPLE_SIZE"));
	tempOpt.SAMPLE_FREQUENCY = json_integer_value(json_object_get(optionsData,"SAMPLE_FREQUENCY"));
	tempOpt.NO_CHANNELS = json_integer_value(json_object_get(optionsData,"NO_CHANNELS"));
	tempOpt.R_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_R"));
	tempOpt.G_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_G"));
	tempOpt.B_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_B"));
	tempOpt.A_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_A"));
	tempOpt.NO_BUTTONS = json_integer_value(json_object_get(optionsData,"NO_BUTTONS"));
	tempOpt.NO_CORPSES= json_integer_value(json_object_get(optionsData,"NO_CORPSES"));
	tempOpt.NO_SPRITES= json_integer_value(json_object_get(optionsData,"NO_SPRITES"));
	tempOpt.NO_SOUNDS= json_integer_value(json_object_get(optionsData,"NO_SOUNDS"));
	tempOpt.QUIT_OFFSET= json_integer_value(json_object_get(optionsData,"QUIT_OFFSET"));
	tempOpt.OTHER_OFFSET= json_integer_value(json_object_get(optionsData,"OTHER_OFFSET"));
	tempOpt.BUTTON_TRANSPARENCY= json_integer_value(json_object_get(optionsData,"BUTTON_TRANSPARENCY"));
	tempOpt.NO_UNITS = json_integer_value(json_object_get(optionsData,"NO_UNITS"));
	tempOpt.SQUAD_SIZE = json_integer_value(json_object_get(optionsData,"SQUAD_SIZE"));
	tempOpt.HP_PER_SIDE = json_integer_value(json_object_get(optionsData,"HP_OF_SIDE"));
	tempOpt.ROF = json_integer_value(json_object_get(optionsData,"ROF"));
	tempOpt.FRAMES_PER_ANIM = json_integer_value(json_object_get(optionsData,"FRAMES_PER_ANIM"));
	tempOpt.ACCURACY_DEVIATION = json_integer_value(json_object_get(optionsData,"ACCURACY_DEVIATION"));
	tempOpt.MG_RANGE = json_integer_value(json_object_get(optionsData,"MG_RANGE"));
	tempOpt.SCALE_FACTOR = json_number_value(json_object_get(optionsData,"SCALE_FACTOR"));
	

	
	return tempOpt;
}

SDL_Window *initSDL(options *opt, int *success)
{

	SDL_Window *temp;
	int SDL_Flags, IMG_Flags;
	SDL_Flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO;
	IMG_Flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if(SDL_Init(SDL_Flags) < 0)
	{
		fprintf(stderr, "SDL_Init has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}
	if(TTF_Init() != 0)
	{
		fprintf(stderr, "TFF_Init has failed : %s \n", TTF_GetError());
		*success = FAIL;
		return NULL;
	}
	if(!(IMG_Init(IMG_Flags) & IMG_Flags))
	{
		fprintf(stderr, "IMG_Init has failed, %s \n", IMG_GetError());
		*success = FAIL;
		return NULL;
	
	}
	if(Mix_OpenAudio(opt->SAMPLE_FREQUENCY, MIX_DEFAULT_FORMAT, opt->NO_CHANNELS, opt->SAMPLE_SIZE) < 0)//first arg : frequency of audio, second arg : format, third arg : Number of audio channels, forth arg : sample size
	{
		fprintf(stderr, "Mix_OpenAudio has failed, %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	
	}
	temp = SDL_CreateWindow(opt->title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, opt->SCREEN_WIDTH, opt->SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!temp)
	{
		fprintf(stderr, "SDL_CreateWindow has failed : %s \n");
		*success = FAIL;
		return NULL;
	
	}
	
	return temp;



}
void deinit()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}
void dumpOptions(options *opt)
{
	fprintf(stderr, "Screen Width : %d \n", opt->SCREEN_WIDTH);
	fprintf(stderr, "Screen Height : %d \n", opt->SCREEN_HEIGHT);

}

SDL_Renderer *createRenderer(SDL_Window *screen, int *success)
{
	SDL_Renderer *temp;
	int Render_Flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	temp = SDL_CreateRenderer(screen, -1, Render_Flags);
	if(!temp)
	{
		fprintf(stderr, "SDL_CreateRenderer has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}

Mix_Music *loadMusic(const char *filename, int *success)
{
	Mix_Music *temp = Mix_LoadMUS(filename);
	if(!temp)
	{
		fprintf(stderr, "Mix_LoadMUS has failed : %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}
Mix_Chunk *loadEffect(const char *filename, int *success)
{
	if(strstr(filename, NO_SOUND))
	{
		return NULL;
	}
	Mix_Chunk *temp = Mix_LoadWAV(filename);
	if(!temp)
	{
		fprintf(stderr, "Mix_LoadWAV has failed : %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}

TTF_Font *LoadFont(const char *filename, int size, int *success)
{
	TTF_Font *temp = TTF_OpenFont(filename, size);
	if(!temp)
	{
		fprintf(stderr, "TTF_OpenFont has failed : %s \n", TTF_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}

SDL_Texture *loadImage(const char *filename, SDL_Renderer *render, SDL_Rect *dimen,  SDL_Texture *original , options *opt, int *success)
{
	SDL_DestroyTexture(original);
	SDL_Surface *temp;
	SDL_Texture *tempTex;
	temp = IMG_Load(filename);
	if(!temp)
	{
		fprintf(stderr, "IMG_LoadBMP has failed: %s \n", IMG_GetError());
		*success = FAIL;
		return NULL;
	
	}
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255,0,255));
	tempTex = SDL_CreateTextureFromSurface(render, temp);
	if(!tempTex)
	{
		fprintf(stderr, "SDL_CreateTextureFromSurface has failed : %s \n", SDL_GetError());
		*success = FAIL;
		return NULL;
	}
	dimen->w = temp->w * opt->SCALE_FACTOR;
	dimen->h = temp->h * opt->SCALE_FACTOR;
	
	SDL_FreeSurface(temp);
	return tempTex;

}

baseEntity *initBaseEntity(const char *filename, SDL_Renderer *render, int *success, options *opt)
{
	baseEntity *temp = malloc(sizeof(baseEntity));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	temp->dimensions.x = 0;
	temp->dimensions.y = 0;
	
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s \n", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	temp->tex = loadImage(filename,render,&(temp->dimensions), temp->tex, opt, success);


	return temp;
}

entity *initEntity(int desiredType, int side,  baseEntity liveAnimation, baseEntity deadAnimation, int *success, options *opt)
{
	entity *temp = malloc(sizeof(entity));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	temp->type = desiredType;
	temp->side = side;
	temp->liveAnimation = liveAnimation.tex;
	temp->posAndHitbox = liveAnimation.dimensions;
	if(deadAnimation.tex != NULL)
	{
		temp->deadAnimation = deadAnimation.tex;
	
	}
	temp->isAnimated = SUCCESS;
	temp->angle = 0;
	return temp;

}

entity *initTitle(baseEntity animation, int *success, options *opt)
{
	entity *temp;
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	temp = initEntity(TITLE, NO_SIDE, animation, animation, success, opt);
	temp->posAndHitbox.y = (opt->SCREEN_HEIGHT / 5);
	temp->posAndHitbox.x = (opt->SCREEN_WIDTH / 3);
	

	return temp;
}
baseEntity **loadButtons(SDL_Renderer *render, int *success, options *opt)
{
	baseEntity **temp = malloc(sizeof( baseEntity *) * opt->NO_BUTTONS);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	int looper;
	char filename[MAX_LINE];
	for(looper = 0; looper < opt->NO_BUTTONS; looper++)
	{
		sprintf(filename, "%s%d.png", opt->buttons_path, looper);
		temp[looper] = initBaseEntity(filename, render, success, opt);
		filename[0] = '\0';
	}
	return temp;


}
baseEntity **loadSprites(SDL_Renderer *render, int *success, options *opt)
{
	baseEntity **temp = malloc(sizeof( baseEntity *) * opt->NO_SPRITES);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	int looper;
	char filename[MAX_LINE];
	for(looper = 0; looper < opt->NO_SPRITES; looper++)
	{
		sprintf(filename, "%s%d.png", opt->sprite_path, looper);
		temp[looper] = initBaseEntity(filename, render, success, opt);
		filename[0] = '\0';
	}
	return temp;

}
baseEntity **loadCorpses(SDL_Renderer *render, int *success, options *opt)
{
	baseEntity **temp = malloc(sizeof( baseEntity *) * opt->NO_CORPSES);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	}
	int looper;
	char filename[MAX_LINE];
	for(looper = 0; looper < opt->NO_CORPSES; looper++)
	{
		sprintf(filename, "%s%d.png", opt->corpses_path, looper);
		temp[looper] = initBaseEntity(filename, render, success, opt);
		filename[0] = '\0';
	}

	return temp;
}
entity **createMenuButtons(baseEntity **buttons, int *success, options *opt)
{
	entity **temp = malloc(sizeof(entity *) * 2);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	temp[0] = initEntity(START, NO_SIDE, *(buttons[STARTBUT]), *(buttons[STARTBUT]), success, opt);
	temp[1] = initEntity(QUIT, NO_SIDE, *(buttons[QUITBUT]), *(buttons[QUITBUT]), success, opt);
	temp[0]->frame.x = INVALID_RECT;
	temp[1]->frame.x = INVALID_RECT;
	temp[0]->posAndHitbox.x = opt->SCREEN_WIDTH / 2;
	temp[1]->posAndHitbox.x = opt->SCREEN_WIDTH / 2;
	temp[0]->posAndHitbox.y = opt->SCREEN_HEIGHT / 2;
	temp[1]->posAndHitbox.y = (opt->SCREEN_HEIGHT / 2) + opt->QUIT_OFFSET;
	return temp;
}

baseEntity *loadMap(const char *filename, SDL_Renderer *render, int *success, options *opt)//(const char *filename, SDL_Renderer *render, SDL_Rect *dimen,  SDL_Texture *original , options *opt, int *success)
{
	baseEntity *temp;
	temp = initBaseEntity(filename, render, success, opt);
	temp->dimensions.w = opt->SCREEN_WIDTH;
	temp->dimensions.h = opt->SCREEN_HEIGHT;
	
	return temp;

}

entity **createGameButtons(baseEntity **buttons, int *success, options *opt)
{
	entity **temp = malloc(sizeof(entity *) * (opt->NO_BUTTONS - 2));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	int looper, yOffset;
	yOffset = 0;
	for(looper = 0; looper < (opt->NO_BUTTONS) - 2; looper++)
	{
		
		temp[looper] = initEntity(looper, NO_SIDE, *(buttons[looper]), *(buttons[looper]), success, opt);
		temp[looper]->posAndHitbox.x = 0;
		temp[looper]->posAndHitbox.y = yOffset;
		yOffset += opt->OTHER_OFFSET;
		SDL_SetTextureAlphaMod(temp[looper]->liveAnimation, opt->BUTTON_TRANSPARENCY);
		temp[looper]->frame.x = INVALID_RECT;
	}

	return temp;

}

entity *loadMachineGun(unitData *mGData, int side, baseEntity *mGTex, int *success, options *opt)
{
	entity *temp = initEntity(mGData->ID, side, *mGTex, *mGTex, success, opt);
	temp->cost = mGData->cost;
	temp->isAnimated = SUCCESS;
	temp->speed = mGData->speed;
	temp->frame.x = INVALID_RECT;
	temp->angle = 90;
	return temp;
}

unitData **loadUnitData(const char *file, int *success, options *opt)
{
	unitData **temp = malloc(sizeof(unitData *) * opt->NO_UNITS);
	json_t *dataHold, *unitEntry;
	json_error_t errorHandle;
	int looper;
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	
	dataHold = json_loads(file,0,&errorHandle);
	
	if(!dataHold)
	{
		fprintf(stderr, "json_loads has failed : %s", errorHandle.text);
		*success = FAIL;
		return NULL;
	}
	
	for(looper = 0; looper < opt->NO_UNITS; looper++)
	{
		temp[looper] = malloc(sizeof(unitData));

		if(!temp[looper])
		{
			fprintf(stderr, "malloc has failed : %s", strerror(errno));
			*success = FAIL;
			return NULL;
		
		}
		unitEntry = json_array_get(dataHold, looper);
		if(!json_is_object(unitEntry))
		{
			fprintf(stderr, "json_array_get has failed, not an object");
		
		}
		temp[looper]->ID = json_integer_value(json_object_get(unitEntry,"ID"));
		temp[looper]->cost = json_integer_value(json_object_get(unitEntry,"cost"));
		temp[looper]->side = json_integer_value(json_object_get(unitEntry,"side"));
		temp[looper]->speed = json_integer_value(json_object_get(unitEntry,"speed"));
		temp[looper]->entrance_filename = json_string_value(json_object_get(unitEntry,"entrance_sound_path"));
		
	
	}
	
	return temp;
}


entity **loadUnits(unitData **data, baseEntity **textures, int *success, options *opt)
{
	entity **temp = malloc(sizeof(entity *) * opt->NO_UNITS);
	int looper;
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	for(looper = 0; looper < opt->NO_UNITS; looper++)
	{
		temp[looper] = initEntity(data[looper]->ID, data[looper]->side, *(textures[looper]), *(textures[looper]), success, opt);
		temp[looper]->entranceSound = loadEffect(data[looper]->entrance_filename, success);
		temp[looper]->speed = data[looper]->speed;
	}
	
	return temp;
}
soldiers *createArmy(unitData *mGData, int side, baseEntity *mGText, int *success, options *opt)
{
	soldiers *temp = malloc(sizeof(soldiers));
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	temp->no_men = 2;
	temp->men = malloc(sizeof(entity) * (temp->no_men));
	temp->men[0] = loadMachineGun(mGData, side, mGText, success, opt);
	temp->men[1] = loadMachineGun(mGData, side, mGText, success, opt);
	fprintf(stderr, "%s\n", mGData->entrance_filename);
	temp->men[0]->entranceSound = loadEffect(mGData->entrance_filename,success);
	temp->men[1]->entranceSound = loadEffect(mGData->entrance_filename,success);
	if(side == BRITISH)
	{
		temp->men[0]->posAndHitbox.x = opt->SCREEN_WIDTH  - opt->OTHER_OFFSET;
		temp->men[1]->posAndHitbox.x = opt->SCREEN_WIDTH  - opt->OTHER_OFFSET;
		temp->men[0]->posAndHitbox.y = opt->SCREEN_HEIGHT - opt->OTHER_OFFSET;
		temp->men[1]->posAndHitbox.y = opt->OTHER_OFFSET;
	
	}
	else if(side == GERMAN)
	{
		temp->men[0]->posAndHitbox.x = opt->OTHER_OFFSET;
		temp->men[1]->posAndHitbox.x = opt->OTHER_OFFSET;
		temp->men[0]->posAndHitbox.y = opt->SCREEN_HEIGHT - opt->OTHER_OFFSET;
		temp->men[1]->posAndHitbox.y = opt->OTHER_OFFSET;
	
	}
	return temp;
}

void newSquad(soldiers *army, options *opt, entity *unitType, baseEntity **corpses, int *success, Mix_Chunk **deathsounds)
{
	army->men = realloc(army->men, (opt->SQUAD_SIZE + army->no_men) * sizeof(entity *));
	int looper, new_size, start, corpse, yCoord, deathSound;
	baseEntity temp;
	temp.tex = unitType->liveAnimation;
	temp.dimensions = unitType->posAndHitbox;
	start = army->no_men; //end of the array
	new_size = army->no_men + opt->SQUAD_SIZE;
	for(looper = start; looper < new_size; looper++)
	{

		corpse = rand() % opt->NO_CORPSES;
		yCoord = rand () % opt->SCREEN_HEIGHT;
		deathSound = rand() % 6;
		army->men[looper] = initEntity(unitType->type, unitType->side, temp, *(corpses[corpse]), success, opt);
		if(unitType->side == GERMAN)
		{
			army->men[looper]->angle = 90;
			army->men[looper]->posAndHitbox.x = opt->OTHER_OFFSET;
		}
		else
		{
			army->men[looper]->angle = 270;
			army->men[looper]->posAndHitbox.x = opt->SCREEN_WIDTH - opt->OTHER_OFFSET;
		}
		army->men[looper]->posAndHitbox.y = yCoord;
		army->men[looper]->entranceSound = unitType->entranceSound;
		army->men[looper]->frame.w = 64;
		army->men[looper]->frame.h = 64;
		army->men[looper]->frame.x = 0;
		army->men[looper]->frame.y = 0;
		army->men[looper]->deathSound = deathsounds[deathSound];
		army->men[looper]->isAnimated = SUCCESS;
		army->men[looper]->speed = unitType->speed;
		army->men[looper]->side = unitType->side;
		army->men[looper]->anim = 0;
		
	
		
	}
	
	Mix_PlayChannel(-1,army->men[looper -1]->entranceSound,0);
	
	army->no_men = new_size;

}
Mix_Chunk **loadDeathSounds(int *success)
{
	Mix_Chunk **temp = malloc(sizeof(Mix_Chunk *) * 6);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	//cba
	temp[0] = loadEffect("audio/death_sound_1.wav", success);
	temp[1] = loadEffect("audio/death_sound_2.wav", success);
	temp[2] = loadEffect("audio/death_sound_3.wav", success);
	temp[3] = loadEffect("audio/death_sound_4.wav", success);
	temp[4] = loadEffect("audio/death_sound_5.wav", success);
	temp[5] = loadEffect("audio/death_sound_6.wav", success);

	
	return temp;

}
void killAll(soldiers *army)
{
	int looper,derp;
	derp = 0;
	double angleOfDeath;
	for(looper = 1; looper < army->no_men; looper++)
	{
		angleOfDeath = rand() % 360;
		if(army->men[looper]->isAnimated != FAIL)
		{
			army->men[looper]->angle = angleOfDeath;
		}
		if(derp < 3)
		{
			if(army->men[looper]->isAnimated != FAIL)
			{
				derp++;
				Mix_PlayChannel(-1,army->men[looper]->deathSound,0);
			}
		}
		fprintf(stderr,"%d", army->no_men);
		army->men[looper]->isAnimated = FAIL;
		
	
	}



}

void newObject(soldiers *army, options *opt, entity *unitType,int *success, Mix_Chunk **sounds, int number, entity *mg, int op, int adj)
{
	
	army->men = realloc(army->men, (number + army->no_men) * sizeof(entity *));
	int looper, new_size, start, corpse, yCoord, deathSound, holdTime;
	static int startTime, endTime;
	startTime = SDL_GetTicks();
	holdTime = startTime - endTime;
	if(holdTime < opt->ROF )
	{
	
		return;
	}
	baseEntity temp;
	temp.tex = unitType->liveAnimation;
	temp.dimensions = unitType->posAndHitbox;
	start = army->no_men; //end of the array
	new_size = army->no_men + number;
	for(looper = start; looper < new_size; looper++)
	{

		army->men[looper] = initEntity(unitType->type, unitType->side, temp, temp, success, opt);
		army->men[looper]->posAndHitbox.y = mg->posAndHitbox.y;
		army->men[looper]->posAndHitbox.x = mg->posAndHitbox.x;
		army->men[looper]->frame.x = INVALID_RECT;
		army->men[looper]->entranceSound = unitType->entranceSound;
		army->men[looper]->deathSound = unitType->entranceSound;
		army->men[looper]->isAnimated = SUCCESS;
		army->men[looper]->speed = unitType->speed;
		army->men[looper]->side = unitType->side;
		army->men[looper]->adj = adj;
		army->men[looper]->op = op;
		army->men[looper]->angle = 0;
		
	
		
	}
	fprintf(stderr, "%d \n", army->no_men);
	Mix_PlayChannel(1,army->men[looper -1]->entranceSound,0);
	endTime = SDL_GetTicks();
	army->no_men = new_size;

}


Mix_Chunk **loadExplosions(int *success)
{
	Mix_Chunk **temp = malloc(sizeof(Mix_Chunk *) * 3);
	if(!temp)
	{
		fprintf(stderr, "malloc has failed : %s", strerror(errno));
		*success = FAIL;
		return NULL;
	
	}
	//cba
	temp[0] = loadEffect("audio/artillery.wav", success);
	temp[1] = loadEffect("audio/lewis_gun.wav", success);
	temp[2] = loadEffect("audio/maxim_gun.wav", success);

	
	return temp;


}