#ifndef DEBUG_H
#define DEBUG_H

void ErrorMessage( const char* message, ... );

#ifdef RX_DEBUG

void DebugMessage( const char* message, ... );
void WarningMessage( const char* message, ... );

#else

#define DebugMessage( message, ... )  \
    ; // no op

#define WarningMessage( message, ... ) \
    ; // no op

#endif // RX_DEBUG

#endif // DEBUG_H
