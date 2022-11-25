#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "string.h"
#include "attribute.h"

typedef struct
{
    String name;
    int hp;
    int maxHp;
    Attribute* attribute;
    int attack;
    int defence;
} Monster;

#endif // _CALC_H_