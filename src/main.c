#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* to-do:
    when the player mines, the block the player chose will be replaced with
   space. currently if the player walks over this space they dont fall down even
   if the block under them is space. i can change this by checking if the player
   is on a space block and if the block under the player is a space. If so, move
   the player down untill they reach the end. this can help the mining down
   feature

   add bigger map, keep window size same, just move the stuff to the left:
    when the player tries to move past the border, increase width:

   add saving and loading

   add new window to right or below with stats, inventory,

   add line of sight, player can only see so far. If the player is in a room the player can completly see the room unless the room is really big or something 
*/

#define WIDTH 60
#define LENGTH 30
#define HEIGHT 10

bool running = true;

typedef struct 
{
  int z, y, x;
  char icon;
} Entity;

//          z        y      x
char room[HEIGHT][LENGTH][WIDTH];
const char blocks[] = {' ', '.', '%', '#', '|', '-'};
int selectedBlock = 0;

void generate(void) 
{
  for (int z = 0; z < HEIGHT; z++) 
  {
    for (int y = 0; y < LENGTH; y++) 
    {
      for (int x = 0; x < WIDTH; x++) 
      {
        if (y == 0 || x == 0 || y == LENGTH - 1 || x == WIDTH - 1) 
        {
          room[z][y][x] = blocks[0]; // air
        } 
        else if (z == 0) 
        {
          room[z][y][x] = blocks[1]; // grass
        } 
        else if (z > 0 && z < 4)   // z 1, z 2, z 3
        {
          room[z][y][x] = blocks[2]; // dirt
        } else if (z >= 4) {
          room[z][y][x] = blocks[3]; // stone
        }
      }
    }
  }
}

void render(WINDOW *win, Entity *player) 
{
  wclear(win);
  box(win, 0, 0);
  mvwprintw(win, 0, 1, "| %d/%d | Selected block: [ %c ] | Dev: semibrackets |", player->z, HEIGHT, blocks[selectedBlock]);
  mvwprintw(win, LENGTH - 1, 1, "| the TERMICRAFT project | ver: 0.0.0-alpha-dev |");

  for (int y = 1; y < LENGTH - 1; y++) 
  {
    for (int x = 1; x < WIDTH - 1; x++) 
    {
      if (y == player->y && x == player->x && player->z >= 0 && player->z < HEIGHT) 
      {
        mvwaddch(win, y, x, player->icon);
      } 
      else 
      {
        switch (room[player->z][y][x]) 
        {
        case '.':
          wattron(win, COLOR_PAIR(1));
          mvwaddch(win, y, x, room[player->z][y][x]);
          wattroff(win, COLOR_PAIR(1));
          break;
        case '%':
          wattron(win, COLOR_PAIR(2));
          mvwaddch(win, y, x, room[player->z][y][x]);
          wattroff(win, COLOR_PAIR(2));
          break;
        case '#':
          wattron(win, COLOR_PAIR(3));
          mvwaddch(win, y, x, room[player->z][y][x]);
          wattroff(win, COLOR_PAIR(3));
          break;
        default:
          mvwaddch(win, y, x, room[player->z][y][x]);
          break;
        }
      }
    }
  }
}

void mineBlock(Entity *player) 
{
  int direction = getch();
  switch (direction) 
  {
  case 'w': // mine north
    if (player->y > 1)
      room[player->z][player->y - 1][player->x] = blocks[0];
    break;
  case 'a': // mine east
    if (player->x > 1)
      room[player->z][player->y][player->x - 1] = blocks[0];
    break;
  case 's': // mine south
    if (player->y < LENGTH - 2)
      room[player->z][player->y + 1][player->x] = blocks[0];
    break;
  case 'd': // mine west
    if (player->x < WIDTH - 2)
      room[player->z][player->y][player->x + 1] = blocks[0];
    break;
  case 'q': // mine northeast
    if (player->y > 1 && player->x > 1)
      room[player->z][player->y - 1][player->x - 1] = blocks[0];
    break;
  case 'e': // mine northwest
    if (player->y > 1 && player->x < WIDTH - 2)
      room[player->z][player->y - 1][player->x + 1] = blocks[0];
    break;
  case 'z': // mine southeast
    if (player->y < LENGTH - 2 && player->x > 1)
      room[player->z][player->y + 1][player->x - 1] = blocks[0];
    break;
  case 'c': // mine southwest
    if (player->y < LENGTH - 2 && player->x < WIDTH - 2)
      room[player->z][player->y + 1][player->x + 1] = blocks[0];
    break;
  case KEY_RIGHT: // mine at player pos
    if (player->z < HEIGHT - 1)
      room[player->z][player->y][player->x] = blocks[0];
    break;
  case 'W': // mine above NOTE: temporary (probably)
    if (player->z > 0)
      room[player->z - 1][player->y][player->x] = blocks[0];
    break;
  case 'S': // mine below NOTE: temporary (probably)
    if (player->z < HEIGHT - 1)
      room[player->z + 1][player->y][player->x] = blocks[0];
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
    if (player->y < LENGTH - 2) room[player->z][player->y + 1][player->x] = blocks[selectedBlock];
    break;
  case 'd': // build east
    if (player->x < WIDTH - 2) room[player->z][player->y][player->x + 1] = blocks[selectedBlock];
    break;
  case 'q': // build northeast
    if (player->y > 1) 
    {
      if (player->x > 1) room[player->z][player->y - 1][player->x - 1] = blocks[selectedBlock];
    }
    break;
  case 'e': // build northwest
    if (player->y > 1) 
    {
      if (player->x < WIDTH - 2) room[player->z][player->y - 1][player->x + 1] = blocks[selectedBlock];
    }
    break;
  case 'z': // build southeast
    if (player->y < LENGTH - 2) 
    {
      if (player->x > 1) room[player->z][player->y + 1][player->x - 1] = blocks[selectedBlock];
    }
    break;
  case 'c': // build southwest
    if (player->y < LENGTH - 2) 
    {
      if (player->x < WIDTH - 2) room[player->z][player->y + 1][player->x + 1] = blocks[selectedBlock];
    }
    break;
  case KEY_RIGHT: // build at players pos
    if (player->z < HEIGHT - 1) room[player->z][player->y][player->x] = blocks[selectedBlock];
    break;
  case 'W': // build above
    if (player->z > 0) room[player->z - 1][player->y][player->x] = blocks[selectedBlock];
    break;
  case 'S':
    if (player->z < HEIGHT - 1) room[player->z + 1][player->y][player->x] = blocks[selectedBlock];
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
    if (player->y + 1 != LENGTH - 1 &&
        room[player->z][player->y + 1][player->x] != '#' &&
        room[player->z][player->y + 1][player->x] != '%')
      player->y++; // move down
    break;
  case 'd': // move west
    if (player->x + 1 != WIDTH - 1 &&
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
    if (player->y - 1 != 0 && player->x + 1 != WIDTH - 1 &&
        room[player->z][player->y - 1][player->x + 1] != '#' &&
        room[player->z][player->y - 1][player->x + 1] != '%') {
      player->y--;
      player->x++;
    }
    break;
  case 'z': // move south east
    if (player->y + 1 != LENGTH - 1 && player->x - 1 != 0 &&
        room[player->z][player->y + 1][player->x - 1] != '#' &&
        room[player->z][player->y + 1][player->x - 1] != '%') {
      player->y++;
      player->x--;
    }
    break;
  case 'c': // move south west
    if (player->y < LENGTH - 2 && player->x < WIDTH - 2 &&
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
    if (player->z < HEIGHT - 1)
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
      if (selectedBlock < sizeof(blocks) - 1) selectedBlock++;
      break;
    case KEY_DOWN:
      if (selectedBlock > 0) selectedBlock--;
      break;
    case KEY_LEFT:
      mineBlock(player);
      break;
    case KEY_RIGHT:
      placeBlock(player);
      break;
    default:
      break;
    }
  }
}

int main(int argc, char *argv[]) 
{
  Entity player;
  player.x = WIDTH / 2;
  player.y = LENGTH / 2;
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

  WINDOW *game = newwin(LENGTH, WIDTH, 1, 1);
  box(game, 0, 0);
  wrefresh(game);

  generate();

  while (running) 
  {
    getInput(&player);
    render(game, &player);
    wrefresh(game);
  }

  endwin();
  return 0;
}
