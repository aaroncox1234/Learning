#include "SDL/SDL.h"
int main( int argc, char* args[] )
{ 
  //Start SDL 
  SDL_Init(SDL_INIT_EVERYTHING); 

  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 
					 32, SDL_SWSURFACE);

  SDL_Delay(2000);

  //Quit SDL 
  SDL_Quit(); 

  return 0; 
}
