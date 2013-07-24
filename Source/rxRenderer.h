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

    struct Vertex2
    {
        float x, y;
    };

    struct Rectangle
    {
        float x, y, w, h;
    };

    struct ColorRGBA
    {
        float r, g, b, a;
    };

    // TODO: Delete everything from multicolor tutorial.
    struct MultiColorVertex2
    {
        Vertex2 position;
        ColorRGBA color;
    };

    void InitOpenGL( float screenWidth, float screenHeight ) throw ( RendererError );
}

#endif // RX_RENDERER_H
