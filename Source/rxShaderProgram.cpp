#include "rxShaderProgram.h"

namespace rx
{
    ShaderProgram::ShaderProgram() throw( RendererError ): programID( 0 ),
                                                           vertexShaderID( 0 ),
                                                           fragmentShaderID( 0 )
    {
        programID = glCreateProgram();

        // Vertex shader

        vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
        const GLchar* vertexShaderSource[] = { "void main() { gl_Position = gl_Vertex; }" };
        glShaderSource( vertexShaderID, 1, vertexShaderSource, NULL );
        glCompileShader( vertexShaderID );

        GLint vertexShaderCompiled = GL_FALSE;
        glGetShaderiv( vertexShaderID, GL_COMPILE_STATUS, &vertexShaderCompiled );
        if( vertexShaderCompiled != GL_TRUE )
        {
            std::string errorMessage = "Failed to compile vertex shader: ";
            errorMessage += vertexShaderID;
            errorMessage += GetVertexShaderLog();
            throw RendererError( errorMessage );
        }

        glAttachShader( programID, vertexShaderID );

        // Fragment shader

        fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
        const GLchar* fragmentShaderSource[] = { "void main() { gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 ); }" };
        glShaderSource( fragmentShaderID, 1, fragmentShaderSource, NULL );
        glCompileShader( fragmentShaderID );

        GLint fragmentShaderCompiled = GL_FALSE;
        glGetShaderiv( fragmentShaderID, GL_COMPILE_STATUS, &fragmentShaderCompiled );
        if( fragmentShaderCompiled != GL_TRUE )
        {
            std::string errorMessage = "Failed to compile fragment shader: ";
            errorMessage += fragmentShaderID;
            errorMessage += GetFragmentShaderLog();
            throw RendererError( errorMessage );
        }

        glAttachShader( programID, fragmentShaderID );

        // Link program

        GLint programSuccess = GL_TRUE;
        glLinkProgram( programID );

        glGetProgramiv( programID, GL_LINK_STATUS, &programSuccess );
        if( programSuccess != GL_TRUE )
        if( fragmentShaderCompiled != GL_TRUE )
        {
            std::string errorMessage = "Failed to link shader program: ";
            errorMessage += programID;
            errorMessage += GetProgramLog();
            throw RendererError( errorMessage );
        }
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

    std::string ShaderProgram::GetProgramLog() const
    {
        std::string result;

        if ( !glIsProgram( programID ) )
        {
            result = "No shader program found with ID: ";
            result += programID;
        }
        else
        {
            result = "Log for shader program:\n";
            result += "\tProgram ID: ";
            result += programID;
            result += "\n";

            int maxLength = 0;
            glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &maxLength );

            char* log = new char[maxLength];
            int logLength = 0;
            glGetProgramInfoLog( programID, maxLength, &logLength, log );

            if ( logLength > 0 )
            {
                result += "\t";
                result += log;
            }

            delete[] log;
        }

        return result;
    }

    std::string ShaderProgram::GetVertexShaderLog() const
    {
        return GetShaderLog( vertexShaderID );
    }

    std::string ShaderProgram::GetFragmentShaderLog() const
    {
        return GetShaderLog( fragmentShaderID );
    }

    std::string ShaderProgram::GetShaderLog( GLuint shaderID ) const
    {
        std::string result;

        if ( !glIsShader( shaderID ) )
        {
            result = "No shader found with ID: ";
            result += shaderID;
        }
        else
        {
            result = "Log for shader:\n";
            result += "\tShader ID: ";
            result += shaderID;
            result += "\n";

            int maxLength = 0;
            glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &maxLength );

            int logLength = 0;
            char* log = new char[maxLength];
            glGetShaderInfoLog( shaderID, maxLength, &logLength, log );

            if( logLength > 0 )
            {
                result += "\t";
                result += log;
            }

            delete[] log;
        }

        return result;
    }
}
