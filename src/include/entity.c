#include "configurations.h"

void initEntity(Entity *e, int z, int y, int x, int hp, int dmg, int def, float dodge, char icon, char *name)
{
  e->z = z;
  e->y = y;
  e->x = x;
  e->health = hp;
  e->damage = dmg;
  e->defence = def;
  e->dodgeChance = dodge;
  e->icon = icon;
  e->name = name;
}

Entity *constructEntity(int z, int y, int x, int hp, int dmg, int def, float dodge, char icon, char *name)
{
  Entity *e = malloc(sizeof(Entity));
  initEntity(e, z, y, x, hp, dmg, def, dodge, icon, name);
  e->name = strdup(name);
  return e;
}

void deconstructEntity(Entity *e)
{
  free(e->name);
  free(e);
}
