#ifndef PARTICLE_H
#define PARTICLE_H

enum class ParticleType 
    {
    STARS,
    };

struct Particle 
    {
    ParticleType type; 
    unsigned int frames;
    unsigned int currentFrame = 0U;
    unsigned int ticks = 0U;
    unsigned int ticksPerFrame;
    unsigned int x, y;
    };

#endif 