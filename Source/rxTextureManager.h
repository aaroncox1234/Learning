#ifndef RX_TEXTURE_MANAGER_H
#define RX_TEXTURE_MANAGER_H

#include <string>
#include <vector>

#include "rxTexture.h"

namespace rx
{
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
