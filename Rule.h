#ifndef RULE_H
#define RULE_H

#include <vector>
#include "Common.h"

struct Condition 
    {
    u8 type;
    u32 related;
    u8 count;
    bool negated;
    bool operator==( const Condition& other ) const
        {
        return type == other.type 
            && related == other.related
            && negated == other.negated;
        }
    };

struct Rule 
    {
    u32 target; // Block ID 
    u32 op; // OperatorType 
    u32 arg; // Noun or property (varies by type)
    u8 flags;
    bool negated; // True when object is NOT whatever
    bool active;
    std::vector<Condition> conditions; // Conditions
    auto isNegationOf (const Rule& other) const -> bool
        {
        return target == other.target
            && op == other.op
            && arg == other.arg
            && negated != other.negated
            && conditions == other.conditions;
        }
    };

#endif 
