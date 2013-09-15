#ifndef DEBUG_H
#define DEBUG_H

#include "BasicTypes.h"
#include "Vector2.h"

#include <SDL.h>

namespace rx
{

void ErrorMessage( const char* message, ... );

void DebugDrawPoint( real x, real y,
                     uint32 r, uint32 g, uint32 b, uint32 a,
                     uint32 duration = -1 );
void DebugDrawPoint( const Vector2& position,
                     uint32 r, uint32 g, uint32 b, uint32 a,
                     uint32 duration = -1 );

void DebugDrawLine( real startX, real startY, real endX, real endY,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    float duration = -1 );
void DebugDrawLine( const Vector2& start, const Vector2& end,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration = -1 );
void DebugDrawLine( const Line& line,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration = -1 );

void DebugDrawRect( real left, real top, real right, real bottom,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration = -1 );
void DebugDrawRect( const Vector2& topLeft, const Vector2& bottomRight,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration = -1 );
void DebugDrawRect( const Rect& rect,
                    uint32 r, uint32 g, uint32 b, uint32 a,
                    uint32 duration = -1 );

void DebugDrawPolygon( const std::vector< Vector2 >& points,
                       uint32 r, uint32 g, uint32 b, uint32 a,
                       uint32 duration = -1 );
void DebugDrawPolygon( const Polygon& polygon,
                       uint32 r, uint32 g, uint32 b, uint32 a,
                       uint32 duration = -1 );

// Called by main()
void DrawDebugShapes( SDL_Renderer* renderer );
void FreeDebugShapes();

#ifdef RX_DEBUG

void DebugMessage( const char* message, ... );
void WarningMessage( const char* message, ... );

#else

#define DebugMessage( message, ... )  \
    ; // no op

#define WarningMessage( message, ... ) \
    ; // no op

#endif // RX_DEBUG

}

#endif // DEBUG_H
