#ifndef _ENTITY_H
#define _ENTITY_H

#include "configurations.h"

typedef struct 
{
  int z, y, x;
  int health, damage, defence;
  float dodgeChance;
  char icon;
  char *name;
} Entity;

void initEntity(Entity *e, int z, int y, int x, int hp, int dmg, int def, float dodge, char icon, char *name);
Entity *constructEntity(int z, int y, int x, int hp, int dmg, int def, float dodge, char icon, char *name);
void deconstructEntity(Entity *e);

#endif