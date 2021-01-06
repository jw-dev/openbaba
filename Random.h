#ifndef RANDOM_H
#define RANDOM_H 

#include <random>

namespace Random 
    {
    auto rollOdds ( float chance ) -> bool;
    auto getRandomInt ( int min, int max ) -> int;
    }

#endif 