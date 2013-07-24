#ifndef RX_SHADERPROGRAM_H
#define RX_SHADERPROGRAM_H

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
#include <vector>

#include "rxRenderer.h"

namespace rx
{
    class ShaderProgram
    {
    public:
        // TODO: Read scott meyers on named constructors and all his stuff on exceptions.
        static ShaderProgram* CreateProgramFromFiles( const std::string& vertexShaderPath,
                                                      const std::string& fragmentShaderPath ) throw( RendererError );

        ~ShaderProgram();

        void Bind() throw( RendererError );
        void Unbind();

        void LoadUniform( const std::string& uniformName ) throw( RendererError );
        void LoadAttribute( const std::string& attributeName ) throw( RendererError );

        void SetUniform1i( const std::string& uniformName,
                           int value );

        void SetUniform4f( const std::string& uniformName,
                           float value1, float value2, float value3, float value4 );

        void SetUniformMatrix4fv( const std::string& uniformMatrixName,
                                  const glm::mat4& matrix );

        void SetAttribute( const std::string& attributeName,
                           GLuint elementsPerAttribute, GLsizei stride, const GLvoid* data );

        void EnableAttribute( const std::string& attributeName );
        void DisableAttribute( const std::string& attributeName );

        std::string GetProgramLog() const;
        std::string GetVertexShaderLog() const;
        std::string GetFragmentShaderLog() const;

    private:
        ShaderProgram();

        static std::string GetShaderLog( GLuint shaderID );

        // Returns shader ID.
        static GLuint LoadShaderFromFile( const std::string& filePath, GLenum shaderType ) throw( RendererError );

        GLuint programID;

        GLuint vertexShaderID;
        GLuint fragmentShaderID;

        // TODO: Get rid of this. The sprite system doesn't need to be extensible.
        std::map<std::string, GLint> uniforms;
        std::map<std::string, GLint> attributes;
    };
}

#endif // RX_SHADERPROGRAM_H
