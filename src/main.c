#include "include/configurations.h"

/* to-do:
   add bigger map, keep window size same, just move the stuff to the left:
    when the player tries to move past the border, increase width or something like that

   add saving and loading, with overwriting

   add line of sight, player can only see so far. If the player is in a room the player can completly see the room unless the room is really big or something 
*/

char room[gameHEIGHT][gameLENGTH][gameWIDTH];
const char blocks[] = {'.', ',', '%', '#', '|', '-', '\\', '/', '$', '^', '&', '*'};
int selectedBlock = 0;
bool running = true;

int main(int argc, char *argv[]) 
{
  srand(time(NULL));
  
  Entity player;
  player.x = gameWIDTH / 2;
  player.y = gameLENGTH / 2;
  player.z = 0;
  player.icon = '@';
  player.name = "{brackets}";

  initscr();

  if (!has_colors()) 
  {
    printw("ERROR: Your terminal doesn't have access to colors!\n");
    getch();
    endwin();
    return 1;
  }

  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

  init_pair(1, COLOR_GREEN, COLOR_BLACK);  // grass
  init_pair(2, COLOR_YELLOW, COLOR_BLACK); // dirt brown?
  init_pair(3, COLOR_WHITE, COLOR_BLACK);  // stone

  WINDOW *game = newwin(gameLENGTH, gameWIDTH, 1, 1); // size y, size x, start y, start x
  WINDOW *name = newwin(nameLENGTH, nameWIDTH, 1, gameWIDTH + 1);
  WINDOW *stats = newwin(statsLENGTH, statsWIDTH, nameLENGTH + 1, gameWIDTH + 1);
  WINDOW *log = newwin(logLENGTH, logWIDTH, gameLENGTH + 1, 1);
  WINDOW *inventory = newwin(inventoryLENGTH, inventoryWIDTH, statsLENGTH + nameLENGTH + 1, gameWIDTH + 1);
  WINDOW *day = newwin(dayLENGTH, dayWIDTH, statsLENGTH + nameLENGTH + inventoryLENGTH + 1, gameWIDTH + 1);
  box(game, 0, 0);
  box(name, 0, 0);
  box(stats, 0, 0);
  box(log, 0, 0);
  box(inventory, 0, 0);
  box(day, 0, 0);
  wrefresh(game);
  wrefresh(name);
  wrefresh(stats);
  wrefresh(log);
  wrefresh(inventory);
  wrefresh(day);
  
  generate();

  while (running) 
  {
    wrefresh(game);
    wrefresh(name);
    wrefresh(stats);
    wrefresh(log);
    wrefresh(inventory);
    wrefresh(day);
    getInput(&player);
    render(game, name, stats, log, inventory, day, &player);
  }

  endwin();
  return 0;
}
