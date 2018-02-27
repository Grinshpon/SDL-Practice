#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdio.h>
#include<string>

//using namespace std;

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

enum KeyPressSurfaces
{
	KEY_PS_DEFAULT,
	KEY_PS_UP,
	KEY_PS_DOWN,
	KEY_PS_LEFT,
	KEY_PS_RIGHT,
	KEY_PS_TOTAL
};

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Surface* gCurrentSurface = NULL;//current image
SDL_Surface* gKeyPressSurfaces[KEY_PS_TOTAL];

SDL_Surface* loadSurface( std::string path )//load individual image
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if(loadedSurface == NULL)
		printf("Unable to load image: %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if(optimizedSurface == NULL)
			printf("Unable to optimize image: %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

bool init()
{
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}
bool loadMedia()
{
	bool success = true;

	gKeyPressSurfaces[KEY_PS_DEFAULT] = loadSurface("imgs/hello_world.bmp");
	if(gKeyPressSurfaces[KEY_PS_DEFAULT] == NULL)
	{
		printf("Unble to load image: %s! SDL Error: %s\n","imgs/hello_world.bmp",SDL_GetError());
		success = false;
	}
	gKeyPressSurfaces[KEY_PS_UP] = loadSurface("imgs/up.bmp");
	if(gKeyPressSurfaces[KEY_PS_UP] == NULL)
	{
		printf("Unble to load image: %s! SDL Error: %s\n","imgs/up.bmp",SDL_GetError());
		success = false;
	}
	gKeyPressSurfaces[KEY_PS_DOWN] = loadSurface("imgs/down.bmp");
	if(gKeyPressSurfaces[KEY_PS_DOWN] == NULL)
	{
		printf("Unble to load image: %s! SDL Error: %s\n","imgs/down.bmp",SDL_GetError());
		success = false;
	}
	gKeyPressSurfaces[KEY_PS_LEFT] = loadSurface("imgs/left.bmp");
	if(gKeyPressSurfaces[KEY_PS_LEFT] == NULL)
	{
		printf("Unble to load image: %s! SDL Error: %s\n","imgs/left.bmp",SDL_GetError());
		success = false;
	}
	gKeyPressSurfaces[KEY_PS_RIGHT] = loadSurface("imgs/right.bmp");
	if(gKeyPressSurfaces[KEY_PS_RIGHT] == NULL)
	{
		printf("Unble to load image: %s! SDL Error: %s\n","imgs/right.bmp",SDL_GetError());
		success = false;
	}
	return success;
}
void close()
{
	SDL_FreeSurface(gCurrentSurface);
	gCurrentSurface = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char* args[])
{
	if(!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		if(!loadMedia())
		{
			printf("Failed to load media!");
		}
		else
		{
			bool quit = false;
			SDL_Event e;
			gCurrentSurface = gKeyPressSurfaces[KEY_PS_DEFAULT];
			while(!quit)//game loop
			{
				while(SDL_PollEvent(&e) != 0)//PollEvent get events from queue. For multiple controlls, make a switch(case) statement?
				{
					if(e.type == SDL_QUIT)
						quit = true;
					else if (e.type == SDL_KEYDOWN)
					{
						switch(e.key.keysym.sym)
						{
							case SDLK_UP:
							gCurrentSurface = gKeyPressSurfaces[KEY_PS_UP];
							break;

							case SDLK_DOWN:
							gCurrentSurface = gKeyPressSurfaces[KEY_PS_DOWN];
							break;

							case SDLK_LEFT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PS_LEFT];
							break;

							case SDLK_RIGHT:
							gCurrentSurface = gKeyPressSurfaces[KEY_PS_RIGHT];
							break;

							default:
							gCurrentSurface = gKeyPressSurfaces[KEY_PS_DEFAULT];
						}
					}
				}
				SDL_Rect stretch;
				stretch.x=0;stretch.y=0;stretch.w=SCREEN_WIDTH;stretch.h=SCREEN_HEIGHT;
				//SDL_BlitSurface blits original size of image, BlitScaled blits scaled image, takes in reference to SDL_Rect
				SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretch);//in loop or out? test
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}
	close();
}
