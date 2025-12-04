#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "configurations.h"
#include "generate.h"

void generate(void) 
{
  int num = 0;
  for (int z = 0; z < gameHEIGHT; z++) 
  {
    for (int y = 0; y < gameLENGTH; y++) 
    {
      for (int x = 0; x < gameWIDTH; x++) 
      {
        if (y == 0 || x == 0 || y == gameLENGTH - 1 || x == gameWIDTH - 1 || z == 0) 
        {
          num = (rand() % 25 + 1);
          switch (num)
          {
            case 5:
              room[z][y][x] = ',';
              break;
            case 25:
              room[z][y][x] = '.';
              break;
            default:
              room[z][y][x] = ' ';
              break;
          }
        }
        else if (z > 0 && z < 4)   // z 1, z 2, z 3
        {
          room[z][y][x] = '%'; // dirt
        } 
        else if (z >= 4) 
        {
          room[z][y][x] = '#'; // stone
        }
      }
    }
  }
}
