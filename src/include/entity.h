#ifndef _ENTITY_H
#define _ENTITY_H

#include "configurations.h"

typedef struct 
{
  int z, y, x;
  int health, damage, defence;
  int dodgeChance;
  char icon;
  char *name;
} Entity;

#endif