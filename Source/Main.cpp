#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdio.h>
#include <stdlib.h>

#include "rxInput.h"
#include "rxTextureManager.h"

using namespace rx;

static Input* input = NULL;
static TextureManager* textureManager = NULL;

static float cameraX = 0.0f;
static float cameraY = 0.0f;

static const Texture* testTexture;

static const bool FULL_SCREEN = false;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

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
    textureManager = new TextureManager();

    testTexture = textureManager->LoadPNG( "test.png" );

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
        printf( "Video query failed: %s\n", SDL_GetError() );
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

    SDL_Surface* surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, videoInfo->vfmt->BitsPerPixel, sdlFlags );
    if ( surface == 0 )
    {
        printf( "Video mode set failed: %s\n", SDL_GetError() );
        Quit( 1 );
    }
}

static void InitOpenGL()
{
    glViewport( 0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f, -1.0f );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    glEnable( GL_TEXTURE_2D );

    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    GLenum error = glGetError();
    if( error != GL_NO_ERROR )
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
        Quit( 1 );
    }
}

static void HandleKeyboardInput()
{
    input->Update();

    if ( input->keys[SDLK_ESCAPE] == KeyState_Pressed )
    {
        input->quit = true;
    }

    if ( input->keys[SDLK_w] == KeyState_Pressed )
    {
        cameraY -= 16.0f;
    }
    if ( input->keys[SDLK_s] == KeyState_Pressed )
    {
        cameraY += 16.0f;
    }
    if ( input->keys[SDLK_a] == KeyState_Pressed )
    {
        cameraX -= 16.0f;
    }
    if ( input->keys[SDLK_d] == KeyState_Pressed )
    {
        cameraX += 16.0f;
    }
}

static void Draw()
{
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glTranslatef( -cameraX, -cameraY, 0.0f );

    glPushMatrix();
    glTranslatef( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f );
    glBegin( GL_QUADS );
    glVertex2f( -50.0f, -50.0f );
    glVertex2f( 50.0f, -50.0f );
    glVertex2f( 50.0f, 50.0f );
    glVertex2f( -50.0f, 50.0f );
    glEnd();
    glPopMatrix();

    testTexture->Draw( SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f );

    glPopMatrix();

    SDL_GL_SwapBuffers( );
}

static void Quit( int exitCode )
{
    if ( input != NULL )
    {
        delete input;
    }

    if ( textureManager != NULL )
    {
        delete textureManager;
    }

    SDL_Quit( );

    exit( exitCode );
}
