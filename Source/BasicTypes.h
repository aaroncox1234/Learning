#ifndef BASICTYPES_H
#define BASICTYPES_H

#include <vector>

#include "TypeDefs.h"
#include "Vector2.h"

namespace rx
{

typedef std::vector< Vector2 > PointList;

struct Line
{
    Line( real startX, real startY, real endX, real endY ) :
        start( startX, startY ),
        end( endX, endY )
    {}

    Line( const Vector2& start, const Vector2& end ) :
        start( start ),
        end( end )
    {}

    Vector2 start;
    Vector2 end;
};

struct Rect
{
    Rect( real left, real top, real right, real bottom ) :
        topLeft( left, top ),
        bottomRight( right, bottom )
    {}

    Rect( const Vector2& topLeft, const Vector2& bottomRight ) :
        topLeft( topLeft ),
        bottomRight( bottomRight )
    {}

    Vector2 topLeft;
    Vector2 bottomRight;
};

struct Polygon
{
    Polygon()
    {}

    Polygon( std::size_t startingSize ) :
        points( startingSize )
    {}

    Polygon( const PointList& pointList ) :
        points( pointList )
    {}

    PointList points;
};

}

#endif // BASICTYPES_H
