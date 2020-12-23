#ifndef RANDOM_H
#define RANDOM_H 

#include <random>

class Random 
    {
public:
    Random ();
    auto rollOdds ( float chance ) -> bool;
    auto getRandomInt ( int min, int max ) -> int;
private: 
    std::random_device m_dev; 
    std::mt19937 m_random;
    };

#endif 