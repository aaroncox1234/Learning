#include "rxRenderer.h"

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace rx
{
    void InitOpenGL( float screenWidth, float screenHeight ) throw( RendererError )
    {
        GLenum glewResult = glewInit();
        if ( glewResult != GLEW_OK )
        {
            std::string errorMessage = "Error initializing glew. Message: ";
            errorMessage += (char*)( glewGetErrorString( glewResult ) );
            throw RendererError( errorMessage );
        }

        if ( !GLEW_VERSION_2_1 )
        {
            std::string errorMessage = "OpenGL 2.1 not supported.";
            throw RendererError( errorMessage );
        }

        glViewport( 0.f, 0.f, screenWidth, screenHeight );

        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

        glEnable( GL_TEXTURE_2D );

        glEnable( GL_BLEND );
        glDisable( GL_DEPTH_TEST );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        GLenum error = glGetError();
        if( error != GL_NO_ERROR )
        {
            std::string errorMessage = "Error initializing OpenGL. Message: ";
            errorMessage += (char*)( gluErrorString( error ) );
            throw RendererError( errorMessage );
        }
    }
}
