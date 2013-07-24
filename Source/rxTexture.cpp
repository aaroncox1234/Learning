#include "rxTexture.h"

#include <glm/gtx/transform.hpp>

namespace rx
{
    struct TextureCoord
    {
        float s, t;
    };

    struct VertexData
    {
        Vertex2 position;
        TextureCoord textureCoord;
    };

    Texture::Texture( GLuint in_id, GLuint in_width, GLuint in_height) : id( in_id ),
                                                                         width( in_width ),
                                                                         height( in_height ),
                                                                         vboID( 0 ),
                                                                         iboID( 0 )
    {
        VertexData vertexData[4];
        glGenBuffers( 1, &vboID );
        glBindBuffer( GL_ARRAY_BUFFER, vboID );
        glBufferData( GL_ARRAY_BUFFER, 4 * sizeof(VertexData), vertexData, GL_DYNAMIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        GLuint indexData[4] = { 0, 1, 2, 3 };
        glGenBuffers( 1, &iboID );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_DYNAMIC_DRAW );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    Texture::~Texture()
    {
        glDeleteBuffers( 1, &vboID );
        glDeleteBuffers( 1, &iboID );
        glDeleteTextures( 1, &id );
    }

    void Texture::Draw( GLfloat x, GLfloat y, const Rectangle& clip, ShaderProgram& shaderProgram ) const
    {
        GLfloat texLeft = clip.x / width;
        GLfloat texRight = ( clip.x + clip.w ) / width;
        GLfloat texTop = clip.y / height;
        GLfloat texBottom = ( clip.y + clip.h ) / height;

        GLfloat quadWidth = clip.w;
        GLfloat quadHeight = clip.h;

        // TODO: store orientation matrix
        glm::mat4 modelViewMatrix = glm::translate<GLfloat>( x, y, 0.0f );
        shaderProgram.SetUniformMatrix4fv( "rxModelViewMatrix", modelViewMatrix );

        VertexData vData[4];

        vData[0].textureCoord.s = texLeft;
        vData[0].textureCoord.t = texTop;
        vData[0].position.x = 0.f;
        vData[0].position.y = 0.f;

        vData[1].textureCoord.s = texRight;
        vData[1].textureCoord.t = texTop;
        vData[1].position.x = quadWidth;
        vData[1].position.y = 0.f;

        vData[2].textureCoord.s = texRight;
        vData[2].textureCoord.t = texBottom;
        vData[2].position.x = quadWidth;
        vData[2].position.y = quadHeight;

        vData[3].textureCoord.s = texLeft;
        vData[3].textureCoord.t = texBottom;
        vData[3].position.x = 0.f;
        vData[3].position.y = quadHeight;

        glBindTexture( GL_TEXTURE_2D, id );

        shaderProgram.EnableAttribute( "rxVertexPosition" );
        shaderProgram.EnableAttribute( "rxTextureCoordinate" );

            glBindBuffer( GL_ARRAY_BUFFER, vboID );
            glBufferSubData( GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData), vData );

            shaderProgram.SetAttribute( "rxVertexPosition", 2, sizeof(VertexData), (GLvoid*)offsetof( VertexData, position ) );
            shaderProgram.SetAttribute( "rxTextureCoordinate", 2, sizeof(VertexData), (GLvoid*)offsetof( VertexData, textureCoord ) );

            glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );

            glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

        shaderProgram.DisableAttribute( "rxVertexPosition" );
        shaderProgram.DisableAttribute( "rxTextureCoordinate" );

        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}

// TODO: slow text rendering with SDL
