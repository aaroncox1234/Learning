#ifndef RX_RENDERER_H
#define RX_RENDERER_H

#include <stdexcept>
#include <string>

namespace rx
{
    class RendererError : public std::runtime_error
    {
    public:
        RendererError( const std::string& errorMessage ) : runtime_error( errorMessage ) {}
    };

    void InitOpenGL( float screenWidth, float screenHeight ) throw ( RendererError );
}

#endif // RX_RENDERER_H
