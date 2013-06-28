#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "rxInput.h"

using namespace rx;

Input* input = NULL;

static const bool FULL_SCREEN = false;
static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

static void InitSDL();
static void InitOpenGL();
static void HandleKeyboardInput();
static void Draw();
static void Quit( int exitCode );

int main( int argc, char* argv[] )
{
    InitSDL();
    InitOpenGL();

    input = new Input();

    while ( !input->quit )
    {
        HandleKeyboardInput();

        Draw();
    }

    Quit( 0 );
}

static void InitSDL()
{
    int result = SDL_Init( SDL_INIT_VIDEO );
    if ( result < 0 )
    {
        // TODO: Create a log class.
        fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError() );
        Quit( 1 );
    }

    const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
    if ( videoInfo == NULL )
    {
        fprintf( stderr, "Video query failed: %s\n", SDL_GetError() );
        Quit( 1 );
    }

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    int sdlFlags = SDL_OPENGL;
    if ( FULL_SCREEN )
    {
        sdlFlags |= SDL_FULLSCREEN;
    }

    SDL_Surface* surface = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, videoInfo->vfmt->BitsPerPixel, sdlFlags );
    if ( surface == 0 )
    {
        fprintf( stderr, "Video mode set failed: %s\n", SDL_GetError() );
        Quit( 1 );
    }
}

static void InitOpenGL()
{
    glShadeModel( GL_SMOOTH );

    glCullFace( GL_BACK );
    glFrontFace( GL_CCW );
    glEnable( GL_CULL_FACE );

    glClearColor( 0, 0, 0, 0 );

    glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    gluPerspective( 60.0, WINDOW_WIDTH / WINDOW_HEIGHT, 1.0, 1024.0 );
}

static void HandleKeyboardInput()
{
    input->Update();

    if ( input->keys[SDLK_ESCAPE] == KeyState_Pressed )
    {
        input->quit = true;
    }
}

static void Draw()
{
    SDL_GL_SwapBuffers( );
}

static void Quit( int exitCode )
{
    if ( input != NULL )
    {
        delete input;
    }

    SDL_Quit( );

    exit( exitCode );
}
