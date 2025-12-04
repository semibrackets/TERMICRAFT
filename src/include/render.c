#include "configurations.h"

#include "render.h"

void render(WINDOW *game, WINDOW *name, WINDOW *stats, WINDOW *log, WINDOW *inventory, WINDOW *day, Entity *player) 
{
  wclear(game);
  wclear(name);
  wclear(stats);
  wclear(log);
  wclear(inventory);
  wclear(day);
  box(game, 0, 0);
  box(name, 0, 0); 
  box(stats, 0, 0);
  box(log, 0, 0);
  box(inventory, 0, 0);
  box(day, 0, 0);

  for (int y = 1; y < gameLENGTH - 1; y++) 
  {
    for (int x = 1; x < gameWIDTH - 1; x++) 
    {
      if (y == player->y && x == player->x && player->z >= 0 && player->z < gameHEIGHT) 
      {
        mvwaddch(game, y, x, player->icon);
      } 
      else 
      {
        if (room[player->z][y][x] == '.' || room[player->z][y][x] == ',') // grass
        {
          wattron(game, COLOR_PAIR(1));
          mvwaddch(game, y, x, room[player->z][y][x]);
          wattroff(game, COLOR_PAIR(1));
        }
        else if (room[player->z][y][x] == '%') // dirt
        {
          wattron(game, COLOR_PAIR(2));
          mvwaddch(game, y, x, room[player->z][y][x]);
          wattroff(game, COLOR_PAIR(2));
        }
        else if (room[player->z][y][x] == '#') // stone
        {
          wattron(game, COLOR_PAIR(3));
          mvwaddch(game, y, x, room[player->z][y][x]);
          wattroff(game, COLOR_PAIR(3));
        }
        else
        {
          mvwaddch(game, y, x, room[player->z][y][x]);
        }
      }
    }
  }
  
  mvwprintw(name, 1, 1, "Hello %s!", player->name); // name

  for (int i = 1; i < player->health + 1; i++)
  {
    mvwaddch(stats, 2, i, "0"); // Working on this, doesnt show anthing in stats window????
  }

  switch (selectedBlock) // inventory
  {
    case 0:
      mvwprintw(inventory, 1, nameWIDTH / 2 - 3, "[%c]%c%c", blocks[selectedBlock], blocks[selectedBlock + 1], blocks[selectedBlock + 2]);
      break;
    case sizeof(&blocks):
      mvwprintw(inventory, 1, nameWIDTH / 2 - 3, "%c%c[%c]", blocks[selectedBlock - 2], blocks[selectedBlock - 1], blocks[selectedBlock]);
      break;
    default: 
      mvwprintw(inventory, 1, nameWIDTH / 2 - 3, "%c[%c]%c", blocks[selectedBlock - 1], blocks[selectedBlock], blocks[selectedBlock + 1]);
      break;
  }
}