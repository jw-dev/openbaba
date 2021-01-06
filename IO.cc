#include "IO.h"

namespace Level 
    {
    auto IO::load ( const std::string& path ) -> Level* 
        {
        Level* level = new Level ();

        std::ifstream in ( path );
        if ( in && in.good () )
            {
            int theWidth; 
            int theHeight;
            int id; 
            int type;
            int x; 
            int y; 
            int direction;
            in >> level->name;
            in >> theWidth;
            in >> theHeight;
            while ( in >> id
                    >> type
                    >> x 
                    >> y
                    >> direction )
                {
                Block block = makeBlock ( id, type, x, y );
                block.direction = direction;
                level->blocks.push_back ( block );
                }
            level->width = (u8) theWidth;
            level->height = (u8) theHeight;
            in.close ();
            }
        else 
            {
            throw std::runtime_error ( "unable to open/read from file" );
            }
        return level;
        }

    auto IO::save ( const std::string& path, const Level* level ) -> void 
        {
        std::ofstream out ( path );
        if (out && out.good ()) 
            {
            const int theWidth = (int) level->width;
            const int theHeight = (int) level->height;
            out << level->name << std::endl; 
            out << theWidth << std::endl;
            out << theHeight << std::endl;
            for (const auto& block: level->blocks) 
                {
                out << static_cast<int>(block.id)
                    << " "
                    << static_cast<int>(block.tile)
                    << " "
                    << static_cast<int>(block.x)
                    << " "
                    << static_cast<int>(block.y)
                    << " "
                    << static_cast<int>(block.direction)
                    << std::endl;
                }
            out.close ();
            }
        else 
            {
            throw std::runtime_error ( "unable to open/write to file" );
            }
        out.close();
        }
    }