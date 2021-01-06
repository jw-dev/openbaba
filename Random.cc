#include "Random.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace Random 
    {
    static std::random_device rd {};
    static std::mt19937 random { rd() }; 

    auto rollOdds ( float chance ) -> bool
        {
        if (chance == 0 ) return false;
        int max = 1 / chance;
        int result = getRandomInt ( 1, (int)max );
        return result == 1;
        }

    auto getRandomInt ( int min, int max ) -> int
        {
        std::uniform_int_distribution<std::mt19937::result_type> dist ( min, max );
        return dist ( random );
        }
    }