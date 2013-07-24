#include <SDL/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#include <glm/gtx/transform.hpp>

#include "rxInput.h"
#include "rxLog.h"
#include "rxRenderer.h"
#include "rxShaderProgram.h"
#include "rxTextureManager.h"

#include <iostream>

// TODO: store shader programs with source, copy to bin folder as a post build step
// TODO: organize the code blocks solution into virtual folders

using namespace rx;

static const bool FULL_SCREEN = false;
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;

static rx::Log* mainLog = NULL;
static Input* input = NULL;
static TextureManager* textureManager = NULL;

static rx::ShaderProgram* shaderProgram = NULL;
static glm::mat4 projectionMatrix = glm::ortho<GLfloat>( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
static glm::mat4 modelViewMatrix = glm::mat4(); // Identity

static float cameraX = 0.0f;
static float cameraY = 0.0f;

static GLuint testVBO = 0;
static GLuint testIBO = 0;

static const Texture* testTexture;

static void InitSDL();
static void HandleKeyboardInput();
static void Draw();
static void Quit( int exitCode );

// TODO: display text
// TODO: fps display
// TODO: reliable profiling

int main( int argc, char* argv[] )
{
    mainLog = new rx::Log( "main_log.txt" );

    InitSDL();

    try
    {
        rx::InitOpenGL( SCREEN_WIDTH, SCREEN_HEIGHT );

        shaderProgram = rx::ShaderProgram::CreateProgramFromFiles( "TestShaderProgram.glvs",
                                                                   "TestShaderProgram.glfs" );

        shaderProgram->LoadUniform( "rxProjectionMatrix" );
        shaderProgram->LoadUniform( "rxModelViewMatrix" );

        shaderProgram->LoadUniform( "rxTextureColor" );
        shaderProgram->LoadUniform( "rxTextureUnit" );

        shaderProgram->LoadAttribute( "rxVertexPosition" );

        shaderProgram->LoadAttribute( "rxTextureCoordinate" );

        shaderProgram->Bind();

        shaderProgram->SetUniformMatrix4fv( "rxProjectionMatrix", projectionMatrix );
        shaderProgram->SetUniformMatrix4fv( "rxModelViewMatrix", modelViewMatrix );

        shaderProgram->SetUniform1i( "rxTextureUnit", 0 );
    }

    catch( RendererError& error )
    {
        mainLog->WriteLine( error.what() );

        Quit( 1 );
    }

    // TODO: Remove this test stuff...
    MultiColorVertex2 quadVertices[4];
    GLuint quadIndices[4];

    quadVertices[ 0 ].position.x = -50.f; quadVertices[ 0 ].position.y = -50.f;
    quadVertices[ 0 ].color.r = 1.f; quadVertices[ 0 ].color.g = 0.f; quadVertices[ 0 ].color.b = 0.f; quadVertices[ 0 ].color.a = 1.f;

    quadVertices[ 1 ].position.x = 50.f; quadVertices[ 1 ].position.y = -50.f;
    quadVertices[ 1 ].color.r = 1.f; quadVertices[ 1 ].color.g = 1.f; quadVertices[ 1 ].color.b = 0.f; quadVertices[ 1 ].color.a = 1.f;

    quadVertices[ 2 ].position.x = 50.f; quadVertices[ 2 ].position.y = 50.f;
    quadVertices[ 2 ].color.r = 0.f; quadVertices[ 2 ].color.g = 1.f; quadVertices[ 2 ].color.b = 0.f; quadVertices[ 2 ].color.a = 1.f;

    quadVertices[ 3 ].position.x = -50.f; quadVertices[ 3 ].position.y = 50.f;
    quadVertices[ 3 ].color.r = 0.f; quadVertices[ 3 ].color.g = 0.f; quadVertices[ 3 ].color.b = 1.f; quadVertices[ 3 ].color.a = 1.f;

    glGenBuffers( 1, &testVBO );
    glBindBuffer( GL_ARRAY_BUFFER, testVBO );
    glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(MultiColorVertex2), quadVertices, GL_STATIC_DRAW );

    quadIndices[0] = 0; quadIndices[1] = 1; quadIndices[2] = 2; quadIndices[3] = 3;

    glGenBuffers( 1, &testIBO );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, testIBO );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), quadIndices, GL_STATIC_DRAW );

    input = new Input();

    // TODO: use a named constructor for this. change to using a sprite batch that returns and managers sprites.
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

// TODO: Create a really simple API for drawing sprites. It should just read a json file with texture
// data/sprite locations and generate a bunch of sprites. It should load the shaders it needs and
// expose some cool effects. The API should be really small, likely just some functions. Underneath,
// most of this OOP stuff shouldn't be necessary. The API doesn't really need to be extensible beyond
// supporting some effects and the ability to draw sprites.

// TODO: test performance of VBO vs pushing verts each render.
// TODO: find a way to count draw calls. know when they happen.
// TODO: test performance of immediate mode matrix stuff vs glm.

static void Draw()
{
    glClear( GL_COLOR_BUFFER_BIT );

    /*shaderProgram->SetUniform4f( "rxPolygonColor", 1.0f, 0.0f, 1.0f, 1.0f );

    modelViewMatrix = glm::translate<GLfloat>( SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f, 0.f );

    glm::mat4 cameraMatrix = glm::translate<GLfloat>( -cameraX, -cameraY, 0.0f );
    modelViewMatrix *= cameraMatrix;

    shaderProgram->SetUniformMatrix4fv( "rxModelViewMatrix", modelViewMatrix );

    shaderProgram->EnableAttribute( "rxMultiColor" );
    shaderProgram->EnableAttribute( "rxVertexPosition" );

    glEnableClientState( GL_VERTEX_ARRAY );
        glBindBuffer( GL_ARRAY_BUFFER, testVBO );
        shaderProgram->SetAttribute( "rxMultiColor", 4, sizeof( MultiColorVertex2 ), (GLvoid*)offsetof( MultiColorVertex2, color ) );
        shaderProgram->SetAttribute( "rxVertexPosition", 2, sizeof( MultiColorVertex2 ), (GLvoid*)offsetof( MultiColorVertex2, position ) );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, testIBO );
        glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );
    glDisableClientState( GL_VERTEX_ARRAY );

    shaderProgram->DisableAttribute( "rxMultiColor" );
    shaderProgram->DisableAttribute( "rxVertexPosition" );
*/
    Rectangle clip;
    clip.x = 0.0f; clip.y = 0.0f;
    clip.w = 200.0f; clip.h = 100.0f;

    shaderProgram->SetUniform4f( "rxTextureColor", 1.0f, 1.0f, 1.0f, 0.5f );

    // TODO: texture should draw from center (er....sprite...)
    testTexture->Draw( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, clip, *shaderProgram );

    SDL_GL_SwapBuffers( );
}

static void Quit( int exitCode )
{
    delete mainLog;
    delete input;
    delete textureManager;
    delete shaderProgram;

    SDL_Quit( );

    exit( exitCode );
}
