#include "rxTexture.h"

namespace rx
{
    Texture::Texture( GLuint in_id, GLuint in_width, GLuint in_height) : id( in_id ),
                                                                         width( in_width ),
                                                                         height( in_height )
    {
    }

    Texture::~Texture()
    {
        glDeleteTextures( 1, &id );
    }

    void Texture::Draw( GLfloat x, GLfloat y ) const
    {
        glBindTexture( GL_TEXTURE_2D, id );
        glPushMatrix();
        glTranslatef( x, y, 0.0f );
        glBegin( GL_QUADS );
        glTexCoord2f( 0.f, 0.f ); glVertex2f( 0.f, 0.f );
        glTexCoord2f( 1.f, 0.f ); glVertex2f( width, 0.f );
        glTexCoord2f( 1.f, 1.f ); glVertex2f( width, height );
        glTexCoord2f( 0.f, 1.f ); glVertex2f( 0.f, height );
        glEnd();
        glPopMatrix();
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}
