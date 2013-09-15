#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "Debug.h"
#include "Vector2.h"

using namespace rx;

const std::string WINDOW_TITLE = "Demo Land";
const uint32 SCREEN_WIDTH = 640;
const uint32 SCREEN_HEIGHT = 480;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

void InitSDL();
void ShutdownSDL();

//Returns true if program should quit.
bool HandleInput();

int main( int argc, char* args[] )
{
	InitSDL();

	bool quit = false;

    while ( !quit )
    {
        quit = HandleInput();

        SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );
        SDL_RenderClear( gRenderer );


        // Update and draw here


        DrawDebugShapes( gRenderer );

        SDL_RenderPresent( gRenderer );
    }

    FreeDebugShapes();

	ShutdownSDL();

	return 0;
}

void InitSDL()
{
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
        ErrorMessage( "Failed to initialize SDL. SDL error: %s", SDL_GetError() );
	}

	// Use linear texture filtering.
	if ( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
    {
        WarningMessage( "Failed to enable linear texture filtering. SDL error: %s", SDL_GetError() );
    }

	gWindow = SDL_CreateWindow( WINDOW_TITLE.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if ( gWindow == NULL )
	{
        ErrorMessage( "Failed to create window. SDL error: %s", SDL_GetError() );
    }

    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if ( gRenderer == NULL )
	{
        ErrorMessage( "Failed to create renderer. SDL erorr: %s", SDL_GetError() );
    }

    int sdlImageFlags = IMG_INIT_PNG;
    if ( !( IMG_Init( sdlImageFlags ) & sdlImageFlags ) )
    {
        ErrorMessage( "Failed to initialize SDL_image. SDL_image error: %s", IMG_GetError() );
    }
}

void ShutdownSDL()
{
    SDL_DestroyRenderer( gRenderer );
    gRenderer = NULL;

	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	IMG_Quit();

	SDL_Quit();
}

bool HandleInput()
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) != 0 )
	{
		if( event.type == SDL_QUIT )
        {
			return true;
		}
    }

    return false;
}
