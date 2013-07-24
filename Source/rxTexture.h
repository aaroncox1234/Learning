#ifndef RX_TEXTURE_H
#define RX_TEXTURE_H

#include <GL/glew.h>
#include <GL/gl.h>

#include "rxRenderer.h"
#include "rxShaderProgram.h"

namespace rx
{
    class Texture
    {
    public:
        Texture( GLuint in_id, GLuint in_width, GLuint in_height );
        ~Texture();

        void Draw( GLfloat x, GLfloat y, const Rectangle& clip, ShaderProgram& shaderProgram ) const;

    private:
        GLuint id;
        GLuint width;
        GLuint height;

        GLuint vboID;
        GLuint iboID;
    };
}

#endif // RX_TEXTURE_H
