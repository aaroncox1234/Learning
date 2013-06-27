#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>
#include <algorithm>

namespace rx
{

enum KeyState
{
    KeyState_None,
    KeyState_Held,
    KeyState_Pressed,
    KeyState_Released
};

class Input
{
public:
    bool quit;
    KeyState keys[SDLK_LAST];

    Input() : quit(false)
    {
        currentKeys = SDL_GetKeyState( &numKeys );
        lastKeys = new Uint8[numKeys];
        std::copy( currentKeys, currentKeys + numKeys, lastKeys );
    }

    ~Input()
    {
        delete [] lastKeys;

        // Note: currentKeys is owned by SDL
    }

    void Update()
    {
        for ( int i = 0; i < numKeys; i++ )
        {
            if ( !lastKeys[i] && currentKeys[i] )
            {
                keys[i] = KeyState_Pressed;
            }
            else if ( lastKeys[i] && currentKeys[i] )
            {
                keys[i] = KeyState_Held;
            }
            else if ( lastKeys[i] && !currentKeys[i] )
            {
                keys[i] = KeyState_Released;
            }
            else
            {
                keys[i] = KeyState_None;
            }
        }

        std::copy(currentKeys, currentKeys + numKeys, lastKeys);

        SDL_Event event;

        while ( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }
    }

private:
    int numKeys;
    Uint8* currentKeys;
    Uint8* lastKeys;
};

}

#endif // INPUT_H
