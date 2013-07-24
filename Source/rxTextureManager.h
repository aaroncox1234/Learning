#ifndef RX_TEXTURE_MANAGER_H
#define RX_TEXTURE_MANAGER_H

#include <string>
#include <vector>

#include "rxTexture.h"

namespace rx
{
    // TODO: don't have a texture manager. have a SpriteSheet that the owner has to delete. Have the SpriteSheet return Sprites. Enforce power of 2 sprite sheets.
    class TextureManager
    {
    public:
        TextureManager();
        ~TextureManager();

        const Texture* LoadPNG( const std::string& filePath );

    private:
        typedef std::vector< Texture* > TextureList;

        TextureList textures;
    };
}

#endif // RX_TEXTURE_MANAGER_H
