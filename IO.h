#ifndef IO_H
#define IO_H

#include <string>
#include <stdexcept>
#include <fstream>
#include "Level.h"

namespace Level 
    {
    class IO
        {
    public:
        virtual auto load (const std::string& path) -> Level*;
        virtual auto save (const std::string& path, const Level* level) -> void;
        };
    }

#endif 