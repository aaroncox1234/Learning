#include "rxTextureManager.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

namespace rx
{
    TextureManager::TextureManager()
    {
    }

    TextureManager::~TextureManager()
    {
        for ( TextureList::iterator itr = textures.begin(); itr != textures.end(); ++itr )
        {
            delete *itr;
        }
    }

    const Texture* TextureManager::LoadPNG( const std::string& filePath )
    {
        SDL_Surface* surface = IMG_Load( filePath.c_str() );
        if ( surface == NULL )
        {
            printf( "SDL failed to load: %s\n", SDL_GetError() );
            return NULL;
        }

        GLenum textureFormat;
        GLint numColors = surface->format->BytesPerPixel;
        if ( numColors == 4 )
        {
            if (surface->format->Rmask == 0x000000ff)
            {
                textureFormat = GL_RGBA;
            }
            else
            {
                textureFormat = GL_BGRA;
            }
        }
        else if ( numColors == 3 )
        {
            if (surface->format->Rmask == 0x000000ff)
            {
                textureFormat = GL_RGB;
            }
            else
            {
                textureFormat = GL_BGR;
            }
        }
        else
        {
            printf( "Warning: %s is not true color.\n", SDL_GetError() );
        }

        GLuint textureID;
        glGenTextures( 1, &textureID );

        glBindTexture( GL_TEXTURE_2D, textureID );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexImage2D( GL_TEXTURE_2D, 0, numColors, surface->w, surface->h, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels );

        Texture* newTexture = new Texture( textureID, surface->w, surface->h );

        textures.push_back( newTexture );

        glBindTexture( GL_TEXTURE_2D, 0 );

        SDL_FreeSurface( surface );

        return newTexture;
    }
}
