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
options initOptions(char *jsonFile, int *success)
{
	fprintf(stderr, "%s", jsonFile);
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
	tempOpt.title = json_string_value(json_object_get(optionsData, "TITLE"));
	tempOpt.SAMPLE_SIZE = json_integer_value(json_object_get(optionsData,"SAMPLE_SIZE"));
	tempOpt.SAMPLE_FREQUENCY = json_integer_value(json_object_get(optionsData,"SAMPLE_FREQUENCY"));
	tempOpt.NO_CHANNELS = json_integer_value(json_object_get(optionsData,"NO_CHANNELS"));
	tempOpt.R_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_R"));
	tempOpt.G_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_G"));
	tempOpt.B_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_B"));
	tempOpt.A_COL = json_integer_value(json_object_get(optionsData,"BG_COLOR_A"));
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

Mix_Music *loadMusic(char *filename, int *success)
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
Mix_Chunk *loadEffect(char *filename, int *success)
{
	Mix_Chunk *temp = Mix_LoadWAV(filename);
	if(!temp)
	{
		fprintf(stderr, "Mix_LoadWAV has failed : %s \n", Mix_GetError());
		*success = FAIL;
		return NULL;
	}

	return temp;
}

TTF_Font *LoadFont(char *filename, int size, int *success)
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

SDL_Texture *loadImage(char *filename, SDL_Renderer *render, SDL_Rect *dimen,  SDL_Texture *original , options *opt, int *success)
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
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255,255,255));
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

baseEntity *initBaseEntity(char *filename, SDL_Renderer *render, int *success, options *opt)
{
	baseEntity *temp = malloc(sizeof(baseEntity));
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
	temp->type = desiredType;
	temp->side = side;
	temp->liveAnimation = liveAnimation.tex;
	temp->posAndHitbox = liveAnimation.dimensions;
	if(deadAnimation.tex != NULL)
	{
		temp->deadAnimation = deadAnimation.tex;
	
	}
	
	return temp;

}
