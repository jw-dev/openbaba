#ifndef ENUMS_H
#define ENUMS_H

#include "Common.h"

enum LevelFlag: u8 
    {
    LEVELFLAG_PARSEWORDS = 0x1,
    LEVELFLAG_WIN = 0x2,
    };

enum Direction: u8 
    {
    DIRECTION_RIGHT,
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_NONE,
    };

enum Tile: u8 
    {
    TILE_ENTITY,
    TILE_WORD,
    TILE_SCENERY,
    };

enum Word: u16
    {
    // Nouns
    WORD_BABA,
    WORD_FLAG,
    WORD_ROCK,
    // Properties
    WORD_YOU,
    WORD_STOP,
    WORD_PUSH,
    WORD_PULL,
    WORD_SWAP,
    WORD_TELE,
    WORD_MOVE,
    WORD_FALL,
    WORD_SHIFT,
    WORD_WIN,
    // Operators 
    WORD_IS,
    WORD_HAS,
    WORD_MAKE,
    WORD_FEAR,
    WORD_EAT,
    WORD_HOLLOW,
    WORD_MIMIC,
    // Conditions
    WORD_ON,
    WORD_FACING,
    WORD_NEAR,
    WORD_WITHOUT,
    WORD_ABOVE,
    WORD_BELOW,
    WORD_FEELING,
    WORD_LONELY,
    WORD_IDLE,
    WORD_POWERED,
    WORD_OFTEN,
    WORD_SELDOM,
    // Unused 
    WORD_UNUSED,
    };

enum Entity: u8 
    {
    ENTITY_BABA = WORD_BABA,
    ENTITY_FLAG,
    ENTITY_ROCK
    };

enum Property: u8
    {
    PROPERTY_YOU = WORD_YOU,
    PROPERTY_STOP,
    PROPERTY_PUSH,
    PROPERTY_PULL,
    PROPERTY_SWAP,
    PROPERTY_TELE,
    PROPERTY_MOVE,
    PROPERTY_FALL,
    PROPERTY_SHIFT,
    PROPERTY_WIN,
    // Many more to come
    };

enum Cond: u8
    {
    CONDITION_ON = WORD_ON, // When two objects overlap. Applies even if one is FLOAT and one isn't.
    CONDITION_FACING, // When object is facing a certain direction OR a certain object
    CONDITION_NEAR, // When object is adjacent to something, applies diagonally
    CONDITION_WITHOUT, // When there is none of the related object in the level
    CONDITION_ABOVE, // When there is the related object below us
    CONDITION_BELOW, // When there is the related object above us
    CONDITION_FEELING, // When the object has a specific property, even via MIMIC
    CONDITION_LONELY, // When the object is overlapping noting
    CONDITION_IDLE, // When pressing the "wait" button without moving. Even if nothing is "YOU", this applies.
    CONDITION_POWERED, // When an object has POWER property
    CONDITION_OFTEN, // Has a 3/4 chance 
    CONDITION_SELDOM // Has a 1/6 chance
    };

enum Operator: u8
    {
    OPERATOR_IS = WORD_IS, // Noun is a different noun (transform) or noun is a property
    OPERATOR_HAS, // Noun has a different noun. When destroyed, will created the associated object(s)
    OPERATOR_MAKE, // Noun will create the associated object(s) every tick
    OPERATOR_FEAR, // Noun will move away from 
    OPERATOR_EAT, // Noun will destroy the associated object *EVEN IF THE OTHER OBJECT IS STOP OR PUSH*
    OPERATOR_FOLLOW, // Noun will always face the associated object 
    OPERATOR_MIMIC, // Noun will inherit properties from the associated object
    };

#endif