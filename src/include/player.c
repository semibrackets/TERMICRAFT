#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>

#include "configurations.h"

void mineBlock(Entity *player) 
{
  int direction = getch();
  switch (direction) 
  {
  case 'w': // mine north
    if (player->y > 1)
      room[player->z][player->y - 1][player->x] = ' ';
    break;
  case 'a': // mine east
    if (player->x > 1)
      room[player->z][player->y][player->x - 1] = ' ';
    break;
  case 's': // mine south
    if (player->y < gameLENGTH - 2)
      room[player->z][player->y + 1][player->x] = ' ';
    break;
  case 'd': // mine west
    if (player->x < gameWIDTH - 2)
      room[player->z][player->y][player->x + 1] = ' ';
    break;
  case 'q': // mine northeast
    if (player->y > 1 && player->x > 1)
      room[player->z][player->y - 1][player->x - 1] = ' ';
    break;
  case 'e': // mine northwest
    if (player->y > 1 && player->x < gameWIDTH - 2)
      room[player->z][player->y - 1][player->x + 1] = ' ';
    break;
  case 'z': // mine southeast
    if (player->y < gameLENGTH - 2 && player->x > 1)
      room[player->z][player->y + 1][player->x - 1] = ' ';
    break;
  case 'c': // mine southwest
    if (player->y < gameLENGTH - 2 && player->x < gameWIDTH - 2)
      room[player->z][player->y + 1][player->x + 1] = ' ';
    break;
  case KEY_DOWN: // mine at player pos
    if (player->z < gameHEIGHT - 1)
      room[player->z][player->y][player->x] = ' ';
    break;
  case 'W': // mine above NOTE: temporary (probably)
    if (player->z > 0)
      room[player->z - 1][player->y][player->x] = ' ';
    break;
  case 'S': // mine below NOTE: temporary (probably)
    if (player->z < gameHEIGHT - 1)
      room[player->z + 1][player->y][player->x] = ' ';
    break;
  default:
    break;
  }
}

void placeBlock(Entity *player) 
{
  int direction = getch();
  switch (direction) 
  {
  case 'w': // build north
    if (player->y > 1) room[player->z][player->y - 1][player->x] = blocks[selectedBlock];
    break;
  case 'a': // build west
    if (player->x > 1) room[player->z][player->y][player->x - 1] = blocks[selectedBlock];
    break;
  case 's': // build south
    if (player->y < gameLENGTH - 2) room[player->z][player->y + 1][player->x] = blocks[selectedBlock];
    break;
  case 'd': // build east
    if (player->x < gameWIDTH - 2) room[player->z][player->y][player->x + 1] = blocks[selectedBlock];
    break;
  case 'q': // build northeast
    if (player->y > 1 && player->x > 1) room[player->z][player->y - 1][player->x - 1] = blocks[selectedBlock];
    break;
  case 'e': // build northwest
    if (player->y > 1 && player->x < gameWIDTH - 2) room[player->z][player->y - 1][player->x + 1] = blocks[selectedBlock];
    break;
  case 'z': // build southeast
    if (player->y < gameLENGTH - 2 && player->x > 1) room[player->z][player->y + 1][player->x - 1] = blocks[selectedBlock];
    break;
  case 'c': // build southwest
    if (player->y < gameLENGTH - 2 && player->x < gameWIDTH - 2) room[player->z][player->y + 1][player->x + 1] = blocks[selectedBlock];
    break;
  case KEY_UP: // build at players pos
    if (player->z < gameHEIGHT - 1) room[player->z][player->y][player->x] = blocks[selectedBlock];
    break;
  case 'W': // build above
    if (player->z > 0) room[player->z - 1][player->y][player->x] = blocks[selectedBlock];
    break;
  case 'S':
    if (player->z < gameHEIGHT - 1) room[player->z + 1][player->y][player->x] = blocks[selectedBlock];
    break;
  default:
    break;
  }
}

void movePlayer(Entity *player, char direction) 
{
  switch (direction) 
  {
  case 'w': // move north
    if (player->y - 1 != 0 &&
        room[player->z][player->y - 1][player->x] != '#' &&
        room[player->z][player->y - 1][player->x] != '%')
      player->y--; // move up
    break;
  case 'a': // move east
    if (player->x - 1 != 0 &&
        room[player->z][player->y][player->x - 1] != '#' &&
        room[player->z][player->y][player->x - 1] != '%')
      player->x--; // move left
    break;
  case 's': // move south
    if (player->y + 1 != gameLENGTH - 1 &&
        room[player->z][player->y + 1][player->x] != '#' &&
        room[player->z][player->y + 1][player->x] != '%')
      player->y++; // move down
    break;
  case 'd': // move west
    if (player->x + 1 != gameWIDTH - 1 &&
        room[player->z][player->y][player->x + 1] != '#' &&
        room[player->z][player->y][player->x + 1] != '%')
      player->x++; // move right
    break;
  case 'q': // move north eats
    if (player->y - 1 != 0 && player->x - 1 != 0 &&
        room[player->z][player->y - 1][player->x - 1] != '#' &&
        room[player->z][player->y - 1][player->x - 1] != '%') {
      player->y--;
      player->x--;
    }
    break;
  case 'e': // move north west
    if (player->y - 1 != 0 && player->x + 1 != gameWIDTH - 1 &&
        room[player->z][player->y - 1][player->x + 1] != '#' &&
        room[player->z][player->y - 1][player->x + 1] != '%') {
      player->y--;
      player->x++;
    }
    break;
  case 'z': // move south east
    if (player->y + 1 != gameLENGTH - 1 && player->x - 1 != 0 &&
        room[player->z][player->y + 1][player->x - 1] != '#' &&
        room[player->z][player->y + 1][player->x - 1] != '%') {
      player->y++;
      player->x--;
    }
    break;
  case 'c': // move south west
    if (player->y < gameLENGTH - 2 && player->x < gameWIDTH - 2 &&
        room[player->z][player->y + 1][player->x] != '#' &&
        room[player->z][player->y + 1][player->x] != '%') {
      player->y++;
      player->x++;
    }
    break;
  case 'W':
    if (player->z > 0)
      player->z--;
    break;
  case 'S':
    if (player->z < gameHEIGHT - 1)
      player->z++;
    break;
  default:
    break;
  }
}

void getInput(Entity *player) 
{
  // if (room[player->z][player->y][player->x] == ' ' && room[player->z + 1][player->y][player->x] == ' ') player->z++; // fall

  int ch = getch();

  if (ch == '\x1b') // ESC
  {
    running = false;
    return;
  }
  else
  {
    if (ch == 'r')
    {
      generate();
    }
    else if (ch == 'q' || ch == 'w' || ch == 'e' || ch == 'a' || ch == 'd' || ch == 'z' || ch == 's' || ch == 'c' || ch == 'W' || ch == 'S') 
    {
      movePlayer(player, ch);
      return;
    } 
    else 
    {
      switch (ch) 
      {
        case KEY_UP:
          placeBlock(player);   
          break;
        case KEY_DOWN:
          mineBlock(player);
          break;
        case KEY_LEFT:
          if (selectedBlock > 0) selectedBlock--;
          break;
        case KEY_RIGHT:
          if (selectedBlock < sizeof(&blocks) - 1) selectedBlock++;
          break;
        default:
          break;
      }
    }
  }
}