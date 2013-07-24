#include "rxShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace rx
{
    ShaderProgram* ShaderProgram::CreateProgramFromFiles( const std::string& vertexShaderPath,
                                                          const std::string& fragmentShaderPath ) throw( RendererError )
    {
        ShaderProgram* result = new ShaderProgram();

        result->programID = glCreateProgram();

        // Vertex shader

        try
        {
            result->vertexShaderID = LoadShaderFromFile( vertexShaderPath, GL_VERTEX_SHADER );
        }
        catch ( const RendererError& error )
        {
            delete result;
            throw error;
        }

        glAttachShader( result->programID, result->vertexShaderID );

        // Fragment shader

        try
        {
            result->fragmentShaderID = LoadShaderFromFile( fragmentShaderPath, GL_FRAGMENT_SHADER );
        }
        catch ( const RendererError& error )
        {
            delete result;
            throw error;
        }

        glAttachShader( result->programID, result->fragmentShaderID );

        // Link program

        GLint programSuccess = GL_TRUE;
        glLinkProgram( result->programID );

        glGetProgramiv( result->programID, GL_LINK_STATUS, &programSuccess );
        if( programSuccess != GL_TRUE )
        {
            std::stringstream errorMessage;
            errorMessage << "Failed to link shader program: " << result->programID << std::endl;
            errorMessage << "\t" << result->GetProgramLog();
            delete result;
            throw RendererError( errorMessage.str() );
        }

        return result;
    }

    ShaderProgram::~ShaderProgram()
    {
        glDeleteProgram( programID );
    }

    void ShaderProgram::Bind() throw( RendererError )
    {
        glUseProgram( programID );

        GLenum error = glGetError();
        if( error != GL_NO_ERROR )
        {
            std::string errorMessage = "Error binding shader. Message: ";
            errorMessage += (char*)( gluErrorString( error ) );
            errorMessage += GetProgramLog();
            throw RendererError( errorMessage );
        }
    }

    void ShaderProgram::Unbind()
    {
        glUseProgram( 0 );
    }

    void ShaderProgram::LoadUniform( const std::string& uniformName ) throw( RendererError )
    {
        GLint location = glGetUniformLocation( programID, uniformName.c_str() );

        if ( location == -1 )
        {
            std::stringstream errorMessage;
            errorMessage << "Invalid shader program uniform: " << uniformName;
            throw RendererError( errorMessage.str() );
        }

        uniforms[uniformName] = location;
    }

    void ShaderProgram::LoadAttribute( const std::string& attributeName ) throw( RendererError )
    {
        GLint location = glGetAttribLocation( programID, attributeName.c_str() );

        if ( location == -1 )
        {
            std::stringstream errorMessage;
            errorMessage << "Invalid shader program uniform: " << attributeName;
            throw RendererError( errorMessage.str() );
        }

        attributes[attributeName] = location;
    }

    void ShaderProgram::SetUniform1i( const std::string& uniformName,
                                      int value )
    {
        glUniform1i( uniforms[uniformName], value );
    }

    void ShaderProgram::SetUniform4f( const std::string& uniformName,
                                      float value1, float value2, float value3, float value4 )
    {
        glUniform4f( uniforms[uniformName], value1, value2, value3, value4 );
    }

    void ShaderProgram::SetUniformMatrix4fv( const std::string& uniformMatrixName,
                                             const glm::mat4& matrix )
    {
        glUniformMatrix4fv( uniforms[uniformMatrixName], 1, GL_FALSE, glm::value_ptr( matrix ) );
    }

    void ShaderProgram::SetAttribute( const std::string& attributeName,
                                      GLuint elementsPerAttribute, GLsizei stride, const GLvoid* data )
    {
        glVertexAttribPointer( attributes[attributeName], elementsPerAttribute, GL_FLOAT, GL_FALSE, stride, data );
    }

    void ShaderProgram::EnableAttribute( const std::string& attributeName )
    {
        glEnableVertexAttribArray( attributes[attributeName] );
    }

    void ShaderProgram::DisableAttribute( const std::string& attributeName )
    {
        glDisableVertexAttribArray( attributes[attributeName] );
    }

    ShaderProgram::ShaderProgram() : programID( 0 ),
                                     vertexShaderID( 0 ),
                                     fragmentShaderID( 0 )
    {}

    std::string ShaderProgram::GetProgramLog() const
    {
        std::stringstream result;

        if ( !glIsProgram( programID ) )
        {
            result << "No shader program found with ID: " << programID;
        }
        else
        {
            result << "Log for shader program:" << std::endl;
            result << "\tProgram ID: " << programID << std::endl;

            int maxLength = 0;
            glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &maxLength );

            char* log = new char[maxLength];
            int logLength = 0;
            glGetProgramInfoLog( programID, maxLength, &logLength, log );

            if ( logLength > 0 )
            {
                result << "\t" << log;
            }

            delete[] log;
        }

        return result.str();
    }

    std::string ShaderProgram::GetVertexShaderLog() const
    {
        return GetShaderLog( vertexShaderID );
    }

    std::string ShaderProgram::GetFragmentShaderLog() const
    {
        return GetShaderLog( fragmentShaderID );
    }

    std::string ShaderProgram::GetShaderLog( GLuint shaderID )
    {
        std::stringstream result;

        if ( glIsShader( shaderID ) )
        {
            result << "Log for shader:\n";
            result << "\tShader ID: " << shaderID << std::endl;

            int maxLength = 0;
            glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &maxLength );

            int logLength = 0;
            char* log = new char[maxLength];
            glGetShaderInfoLog( shaderID, maxLength, &logLength, log );

            if( logLength > 0 )
            {
                result << "\t" << log;
            }

            delete[] log;
        }
        else
        {
            result << "No shader found with ID: " << shaderID;
        }

        return result.str();
    }

    GLuint ShaderProgram::LoadShaderFromFile( const std::string& filePath, GLenum shaderType ) throw( RendererError )
    {
        GLuint shaderID = 0;

        std::ifstream sourceFile( filePath.c_str() );

        if( sourceFile )
        {
            shaderID = glCreateShader( shaderType );

            std::string shaderString;
            shaderString.assign( std::istreambuf_iterator<char>( sourceFile ), std::istreambuf_iterator<char>() );

            const char* shaderSource = shaderString.c_str();
            glShaderSource( shaderID, 1, (const GLchar**)&shaderSource, NULL );

            glCompileShader( shaderID );

            GLint shaderCompiled = GL_FALSE;
            glGetShaderiv( shaderID, GL_COMPILE_STATUS, &shaderCompiled );
            if( shaderCompiled != GL_TRUE )
            {
                std::stringstream errorMessage;
                errorMessage << "Failed to compile shader. ID: " << shaderID << ". Type: " << shaderType << std::endl;
                errorMessage << "\tFile path: " << filePath;
                errorMessage << "\tLog: " << GetShaderLog( shaderID );
                glDeleteShader( shaderID );
                throw RendererError( errorMessage.str() );
            }
        }
        else
        {
            std::stringstream errorMessage;
            errorMessage << "Failed to load shader file: " << filePath;
            throw RendererError( errorMessage.str() );
        }

        return shaderID;
    }
}
