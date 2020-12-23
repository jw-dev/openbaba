#include "Random.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


Random::Random ()
  : m_dev(),
    m_random (m_dev())
    {
    }

auto Random::rollOdds ( float chance ) -> bool
    {
    if (chance == 0 ) return false;
    int max = 1 / chance;
    int result = Random::getRandomInt ( 1, (int)max );
    return result == 1;
    }

auto Random::getRandomInt ( int min, int max ) -> int
    {
    std::uniform_int_distribution<std::mt19937::result_type> dist ( min, max );
    return dist ( m_random );
    }