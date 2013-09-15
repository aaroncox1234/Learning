#include "Debug.h"

#include <cassert>
#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <list>

namespace rx
{

void ErrorMessage( const char* message, ... )
{
    va_list argList;
    va_start( argList, message );

    char buffer[256];
    vsprintf( buffer, message, argList );
    std::string formattedMessage = buffer;

    va_end( argList );

    DebugMessage( formattedMessage.c_str() );

    int messageBoxResult = SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", formattedMessage.c_str(), NULL);
    if ( messageBoxResult < 0 )
    {
        std::string errorMessage = "Failed to show SDL message box. Error message: ";
        errorMessage += SDL_GetError();

        DebugMessage( errorMessage.c_str() );
    }

    assert(0);
}


struct Color
{
    Color ( uint32 r, uint32 g, uint32 b, uint32 a ) :
        r( r ),
        g( g ),
        b( b ),
        a( a )
    {}

    uint32 r, g, b, a;
};

class DebugShape
{
public:
    DebugShape( const Color& color, uint32 msDuration ) :
        color( color )
    {
        endTime = SDL_GetTicks() + msDuration;
    }

    virtual ~DebugShape() {}

    virtual void Draw( SDL_Renderer* renderer ) = 0;

    bool HasExpired()
    {
        return SDL_GetTicks() >= endTime;
    }

protected:
    Color color;
    uint32 endTime;
};

class DebugPoint : public DebugShape
{
public:
    DebugPoint( const Vector2& position, const Color& color, uint32 endTime ) :
        DebugShape( color, endTime),
        position( position )
    {}

    virtual void Draw( SDL_Renderer* renderer )
    {
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderDrawPoint( renderer, position.x, position.y );
    }

private:
    Vector2 position;
};

class DebugLine : public DebugShape
{
public:
    DebugLine( const Line& line, const Color& color, uint32 endTime ) :
        DebugShape( color, endTime),
        line( line )
    {}

    virtual void Draw( SDL_Renderer* renderer )
    {
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderDrawLine( renderer, line.start.x, line.start.y, line.end.x, line.end.y );
    }

private:
    Line line;
};

class DebugRect : public DebugShape
{
public:
    DebugRect( const Rect& rect, const Color& color, uint32 endTime ) :
        DebugShape( color, endTime),
        rect( rect )
    {}

    virtual void Draw( SDL_Renderer* renderer )
    {
        SDL_Rect sdlRect;
        sdlRect.x = rect.topLeft.x;
        sdlRect.y = rect.topLeft.y;
        sdlRect.w = rect.bottomRight.x;
        sdlRect.h = rect.bottomRight.y;

        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );
		SDL_RenderDrawRect( renderer, &sdlRect );
    }

private:
    Rect rect;
};

class DebugPolygon : public DebugShape
{
public:
    DebugPolygon( const Polygon& polygon, const Color& color, uint32 endTime ) :
        DebugShape( color, endTime),
        polygon( polygon )
    {}

    virtual void Draw( SDL_Renderer* renderer )
    {
        SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );

        uint32 numPoints = polygon.points.size();

		for ( uint32 index = 1; index < numPoints; ++index )
		{
            const Vector2& start = polygon.points[index - 1];
            const Vector2& end = polygon.points[index];

            SDL_RenderDrawLine( renderer, start.x, start.y, end.x, end.y );
		}

		const Vector2& start = polygon.points[numPoints - 1];
        const Vector2& end = polygon.points[0];

        SDL_RenderDrawLine( renderer, start.x, start.y, end.x, end.y );
    }

private:
    Polygon polygon;
};

class DebugDraw
{
public:
    void Clear()
    {
        for ( ShapeList::iterator itr = shapes.begin(); itr != shapes.end(); ++itr )
        {
            delete (*itr);
        }
        shapes.clear();
    }

    void Draw( SDL_Renderer* renderer )
    {
        for ( ShapeList::iterator itr = shapes.begin(); itr != shapes.end(); )
        {
            (*itr)->Draw( renderer );

            if ( (*itr)->HasExpired() )
            {
                delete (*itr);
                shapes.erase( itr++ );
            }
            else
            {
                ++itr;
            }
        }
    }

    void AddShape( DebugShape* shape )
    {
        shapes.push_back( shape );
    }

private:
    typedef std::list< DebugShape* > ShapeList;

    ShapeList shapes;
};

static DebugDraw debugDraw;

void DebugDrawPoint( real x, real y,
                     uint32 r, uint32 g, uint32 b, uint32 a,
                     uint32 duration )
{
    DebugDrawPoint( Vector2( x, y ), r, g, b, a, duration );
}

void DebugDrawPoint( const Vector2& position,
                     uint32 r, uint32 g, uint32 b, uint32 a,
                     uint32 duration )
{
    debugDraw.AddShape( new DebugPoint( position, Color( r, g, b, a ), duration ) );
}

void DebugDrawLine( real startX, real startY, real endX, real endY,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    float duration )
{
    DebugDrawLine( Line( startX, startY, endX, endY ), r, g, b, a, duration );
}

void DebugDrawLine( const Vector2& start, const Vector2& end,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration )
{
    DebugDrawLine( Line( start, end ), r, g, b, a, duration );
}

void DebugDrawLine( const Line& line,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration )
{
    debugDraw.AddShape( new DebugLine( line, Color( r, g, b, a ), duration ) );
}

void DebugDrawRect( real left, real top, real right, real bottom,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration )
{
    DebugDrawRect( Rect( left, top, right, bottom ), r, g, b, a, duration );
}

void DebugDrawRect( const Vector2& topLeft, const Vector2& bottomRight,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration )
{
    DebugDrawRect( Rect( topLeft, bottomRight ), r, g, b, a, duration );
}

void DebugDrawRect( const Rect& rect,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration )
{
    debugDraw.AddShape( new DebugRect( rect , Color( r, g, b, a ), duration ) );
}

void DebugDrawPolygon( const std::vector< Vector2 >& points,
                       uint32 r, uint32 g, uint32 b, uint32 a,
                       uint32 duration )
{
    DebugDrawPolygon( Polygon( points ), r, g, b, a, duration );
}

void DebugDrawPolygon( const Polygon& polygon,
                       uint32 r, uint32 g, uint32 b, uint32 a,
                       uint32 duration )
{
    debugDraw.AddShape( new DebugPolygon( polygon, Color( r, g, b, a ), duration ) );
}

void DrawDebugShapes( SDL_Renderer* renderer )
{
    debugDraw.Draw( renderer );
}

void FreeDebugShapes()
{
    debugDraw.Clear();
}

#ifdef RX_DEBUG

class DebugLog
{
public:
    DebugLog()
    {
        outputFile.open( "DebugLog.txt" );
    }

    ~DebugLog()
    {
        outputFile.close();
    }

    void WriteLine( const std::string& message )
    {
        outputFile << message << std::endl;
    }

private:
    std::ofstream outputFile;
};

static DebugLog debugLog;

void DebugMessage( const char* message, ... )
{
    va_list argList;
    va_start( argList, message );

    char buffer[256];
    vsprintf( buffer, message, argList );
    std::string formattedMessage = buffer;

    va_end( argList );

    debugLog.WriteLine( formattedMessage );
}

void WarningMessage( const char* message, ... )
{
    va_list argList;
    va_start( argList, message );

    char buffer[256];
    vsprintf( buffer, message, argList );
    std::string formattedMessage = buffer;

    va_end( argList );

    debugLog.WriteLine( formattedMessage );
    assert( 0 );
}

}

#endif // RX_DEBUG
