#ifndef _PLAYER_H
#define _PLAYER_H

#include "configurations.h"

// bool airborn = false; // implement jumping with space bar

void mineBlock(Entity *player);
void placeBlock(Entity *player);
void movePlayer(Entity *player, char direction);
void getInput(Entity *player);

#endif
