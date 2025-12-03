#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* to-do:
   add bigger map, keep window size same, just move the stuff to the left:
    when the player tries to move past the border, increase width or something like that

   add saving and loading, with overwriting

   add new window to right or below with stats, inventory,

   add line of sight, player can only see so far. If the player is in a room the player can completly see the room unless the room is really big or something 
*/

#define gameWIDTH 70 // x
#define gameLENGTH 20 // y
#define gameHEIGHT 10 // z

#define inventoryWIDTH 30
#define inventoryLENGTH 5

#define statusWIDTH 15
#define statusLENGTH 10

bool running = true;
// bool airborn = false;

typedef struct 
{
  int z, y, x;
  int health, damage, defence;
  int dodgeChance;
  char icon;
} Entity;

//          z        y      x
char room[gameHEIGHT][gameLENGTH][gameWIDTH];
const char blocks[] = {'.', ',', '%', '#', '|', '-', '\\', '/', '$', '^', '&', '*'};
int selectedBlock = 0;

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

void render(WINDOW *game, WINDOW *inventory, WINDOW *status, Entity *player) 
{
  wclear(game);
  wclear(inventory);
  wclear(status);
  box(game, 0, 0);
  box(inventory, 0, 0); 
  box(status, 0, 0);
  
  mvwprintw(game, 0, 1, "| %d/%d | ver: 0.0.0-alpha-dev |", player->z, gameHEIGHT); // game
  mvwprintw(inventory, 0, inventoryWIDTH / 2 - 7, "| inventory |"); // inv
  mvwprintw(status, 0, 2, "| status |"); // status

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
  
  if (selectedBlock == 0)
  {
    mvwprintw(inventory, 2, inventoryWIDTH / 2 - 3, "[%c] %c %c", blocks[selectedBlock], blocks[selectedBlock + 1], blocks[selectedBlock + 2]);
  }
  else if (selectedBlock == sizeof(blocks) - 1)
  {
    mvwprintw(inventory, 2, inventoryWIDTH / 2 - 3, "%c %c [%c]", blocks[selectedBlock - 2], blocks[selectedBlock - 1], blocks[selectedBlock]);
  }
  else
  {
    mvwprintw(inventory, 2, inventoryWIDTH / 2 - 3, "%c [%c] %c", blocks[selectedBlock - 1], blocks[selectedBlock], blocks[selectedBlock + 1]);
  }
}

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
  if (room[player->z][player->y][player->x] == ' ' && room[player->z + 1][player->y][player->x] == ' ') player->z++; // fall

  int ch = getch();

  if (ch == '\x1b') // ESC
  {
    running = false;
    return;
  }
  else if (ch == 'r')
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
        if (selectedBlock < sizeof(blocks) - 1) selectedBlock++;
        break;
        
      default:
        break;
    }
  }
}

int main(int argc, char *argv[]) 
{
  srand(time(NULL));
  
  Entity player;
  player.x = gameWIDTH / 2;
  player.y = gameLENGTH / 2;
  player.z = 0;
  player.icon = '@';

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
  init_pair(4, COLOR_BLACK, COLOR_BLACK);  // bedrock

  WINDOW *game = newwin(gameLENGTH, gameWIDTH, 1, 1);
  WINDOW *inventory = newwin(inventoryLENGTH, inventoryWIDTH, 1, gameWIDTH + 1);
  WINDOW *status = newwin(statusLENGTH, statusWIDTH, inventoryLENGTH + 1, gameWIDTH + 1);
  box(game, 0, 0);
  box(inventory, 0, 0);
  box(status, 0, 0);
  wrefresh(game);
  wrefresh(inventory);
  wrefresh(status);
  
  generate();

  while (running) 
  {
    getInput(&player);
    render(game, inventory, status, &player);
    wrefresh(game);
    wrefresh(inventory);
    wrefresh(status);
  }

  endwin();
  return 0;
}
