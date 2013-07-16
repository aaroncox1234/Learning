#ifndef RX_SHADERPROGRAM_H
#define RX_SHADERPROGRAM_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

#include "rxRenderer.h"

namespace rx
{
    class ShaderProgram
    {
    public:
        ShaderProgram() throw( RendererError );
        ~ShaderProgram();

        void Bind() throw( RendererError );
        void Unbind();

        std::string GetProgramLog() const;
        std::string GetVertexShaderLog() const;
        std::string GetFragmentShaderLog() const;

    private:
        std::string GetShaderLog( GLuint shaderID ) const;

        GLuint programID;

        GLuint vertexShaderID;
        GLuint fragmentShaderID;
    };
}

#endif // RX_SHADERPROGRAM_H
