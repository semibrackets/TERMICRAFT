#ifndef _CONFIGURATIONS_H
#define _CONFIGURATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#include "generate.h" 
#include "entity.h"
#include "render.h"
#include "player.h"

#define gameWIDTH 100 // game window X
#define gameLENGTH 30 // Game window Y
#define gameHEIGHT 10 // game window Z

#define nameWIDTH 40 // name window X 
#define nameLENGTH 3 // name window Y

#define statsWIDTH (nameWIDTH / 2) // status window X
#define statsLENGTH 10 // status window Y

#define logWIDTH gameWIDTH // log window X
#define logLENGTH 10 // log window Y

#define inventoryWIDTH nameWIDTH // inventory window x
#define inventoryLENGTH 20 // inventory window y

#define dayWIDTH nameWIDTH
#define dayLENGTH 3

extern char room[gameHEIGHT][gameLENGTH][gameWIDTH];

extern const char blocks[]; // will be changed to an inventory when i add survival mode
extern int selectedBlock; // same for this (probably)

extern bool running;

extern void generate(void);

#endif