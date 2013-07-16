#ifndef RX_TEXTURE_H
#define RX_TEXTURE_H

#include <GL/gl.h>

namespace rx
{
    class Texture
    {
    public:
        Texture( GLuint in_id, GLuint in_width, GLuint in_height );
        ~Texture();

        void Draw( GLfloat x, GLfloat y ) const;

    private:
        GLuint id;
        GLuint width;
        GLuint height;
    };
}

#endif // RX_TEXTURE_H
