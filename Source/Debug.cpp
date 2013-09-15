#include "Debug.h"

#include <stdarg.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cassert>

#include <SDL.h>

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

#endif // RX_DEBUG
