#include "Parser.h"

namespace Level 
    {
    auto Parser::parse (const Level* level) -> std::vector<Rule>
        {
        // <CONDITION1,CONDITION2,> [NOUN] [OPERATOR<ARG>] [PROPERTY1<AND PROPERTY2<AND PROPERTYN...>]
        // LONELY AND IDLE BABA NEAR KEKE IS DEFEAT AND WIN
        // CONDITION: LONELY
        // CONDITION: IDLE
        // NOUN: BABA
        // OPERATOR: NEAR [KEKE]
        // PROPERTY: DEFEAT
        // PROPERTY: WIN
        const std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        std::vector <Rule> rules;

        enum State: u8
            {
            STATE_BEGIN,
            STATE_NOUN,
            STATE_OPERATOR,
            STATE_PROPERTY,
            STATE_FINISH,
            STATE_ERROR,
            };

        for ( const auto& block: level->blocks )
            {
            for ( u8 xInc: { 0 , 1 })
                for ( u8 yInc: { 0, 1 })
                    {
                    if ( xInc == yInc ) continue;
                    
                    Rule rule;
                    rule.active = true;

                    u8 x = block.x;
                    u8 y = block.y;
                    u8 state = STATE_BEGIN;
                    bool done = false;
                    for (;;)
                        {
                        const Block * block = nullptr; 
                        // Find block in this position
                        for ( const Block& b : level->blocks )
                            {
                            if ( b.x == x && b.y == y && b.isWord() )
                                {
                                block = &b;
                                break;
                                }
                            }
                        if ( block == nullptr && state != STATE_PROPERTY && state != STATE_FINISH)
                            state = STATE_ERROR;

                        switch (state) 
                            {
                            case STATE_BEGIN: 
                                if ( block->isNoun() )
                                    {
                                    rule.target = block->id;
                                    state = STATE_NOUN;
                                    }
                                else 
                                    state = STATE_ERROR;
                                break;
                            case STATE_NOUN: 
                                if ( block->isOperator() )
                                    {
                                    rule.op = block->id;
                                    state = STATE_OPERATOR;
                                    }
                                else 
                                    state = STATE_ERROR;
                                break;
                            case STATE_OPERATOR: 
                                if ( block->isNoun() ) 
                                    {
                                    rule.flags = TILE_ENTITY;
                                    rule.arg = block->id;
                                    state = STATE_PROPERTY;
                                    }
                                else if ( block->isProp() ) 
                                    {
                                    rule.flags = TILE_WORD;
                                    rule.arg = block->id;
                                    state = STATE_PROPERTY;
                                    }
                                else 
                                    state = STATE_ERROR;
                                break;
                            case STATE_PROPERTY:
                                state = STATE_FINISH;
                                break;
                            case STATE_ERROR:
                                done = true;
                                break;
                            case STATE_FINISH: 
                                for ( Rule& other: rules ) 
                                    {
                                    if ( other.isNegationOf ( rule )) 
                                        {
                                        // We need to disable one of these rules. We should disable the one that is positive rule
                                        // For example, if we have BABA IS YOU and BABA IS NOT YOU then BABA IS YOU should be disabled
                                        if ( other.negated ) rule.active = false;
                                        else other.active = false;
                                        }
                                    }
                                rules.push_back ( rule );
                                rule = Rule (); // reset
                                done = true;
                                break;
                            }
                        if ( done ) 
                            break ;
                        // printf ("   x %d, y %d, %s, state %d\n", x, y, block? block->name.c_str(): "<NULL>", state);
                        x += xInc;
                        y += yInc;
                        }
                    }
            }

        const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "parsing finished in " 
                << std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count() 
                << "ms ("
                << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count()
                << "µs)" 
                << std::endl; 

        return rules;
        }
    }