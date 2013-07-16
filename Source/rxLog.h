#ifndef RX_LOG_H
#define RX_LOG_H

#include <iostream>
#include <fstream>
#include <string>

namespace rx
{
    class Log
    {
    public:
        Log( const std::string& filePath )
        {
            file.open( filePath.c_str() );
        }

        ~Log()
        {
            file.close();
        }

        void WriteLine( const std::string& text )
        {
            file << text << std::endl;
        }

    private:
        std::ofstream file;
    };
}

#endif // RX_LOG_H
