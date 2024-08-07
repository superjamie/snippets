#include <stdio.h>  // include stdio for printf
#include <stdlib.h>  // include stdlib for rand
#include <time.h>  // include time to seed the rng
#include <curses.h>  // include curses for getch

// this was a small obfuscated roguelike which fit in 512 bytes of source
// http://locklessinc.com/articles/512byte_roguelike/
// i've gone through and commented the de-obfuscated source to understand it better
// compile with:
// gcc -O2 monstercaves.c -o monstercaves -lcurses

#define SIZEX 128  // tell the pre-compiler the map can be this many characters across
#define SIZEY 64  // tell the pre-compiler the map can be this many characters down
static char map[SIZEY][SIZEX];  // declare a static character array of size Y down and X across

#define MMAX  50  // tell the pre-compiler we'll be making up to 50 monsters
static int mx[MMAX];  // declare a static global array which stores MMAX integers indicating monster's X positions
static int my[MMAX];  // declare a static global array which stores MMAX integers indicating monster's Y positions
static char mc[MMAX]; // declare a static global array which stores MMAX characters to show monsters on the map
static int mhp[MMAX]; // declare a static global array which stores MMAX characters representing the monster's hitpoints

static void draw_region(int x1, int x2, int y1, int y2, char c) // function to put the map characters in the right place
{
  int x, y; // define two local variables to count each character we need to draw
  
  for (y = y1; y < y2; y++)  // for each iteration of y1 which is smaller than y2
  {
    for (x = x1; x < x2; x++)  // for each iteration of x1 which is smaller than x2
    {
      map[y][x] = c;  // store this character in the map array
    }
  }
}

static int rr(int v1, int v2, int r)  // function to handle random numbers in some way
{
  return (r % (v1 - v2)) + v1;  // take r, modulo it against v1-v2, then add v1 to that
}

static void addm(int x, int y, int hp, char c)  // function to place monster statistics into their arrays
{
  int i;  // counter
  
  for (i = 0; i < MMAX; i++)  // create monsters up to the number of MMAX monsters
  {
    if (!mc[i])  // if there's not a character for this monster
    {
      mx[i] = x;  // put monster i at location X
      my[i] = y;  // put monster i at location Y
      mhp[i] = hp;  // give monster i this many hitpoints
      mc[i] = c;  // monster i can be represented by this character
      return;  // ?? unsure if this returns addm or just the for loop
    }
  }
}

static int getm(int x, int y)  // function to check if there is a monster on a square
{
  int i;  // counter
  for (i = 0; i < MMAX; i++)  // check all monsters up to MMAX
  {
    if ((mx[i] == x) && (my[i] == y)) return i;  // if there is a monster here, return the monster's number
  }
  
  return -1;  // if there is no monster here, return -1
}

static void rand_place(char c, int hp)  // function to randomly place monsters on the map
{
  do
  {
    int x = rand()&127;  // local integer variable x equals random number bitwise AND'ed against 127 ?? (shouldn't this be SIZEX -1 ?)
    int y = rand()&63;   // local integer variable y equals random number bitwise AND'ed against 63 ??  (shouldn't this be SIZEy -1 ?)
    
    if (map[y][x] == ' ')  // if there's empty floor at this location
    {
      map[y][x] = c;  // place character c at this location
      addm(x, y, hp, c);  // place statistics into the monster array
      return;  // get out of this function
    }
  }
  while(1);  // if we tried to place something on an occupied tile, keep trying until we get a blank floor space
}

static void init_map(void)  // function to generate the map, player, and monsters
{
  int i, x1, x2, y1, y2;  // make a local counter, and two integer variables for each direction
  
  draw_region(0, SIZEX - 1, 0, SIZEY, '.');  // fill the map array with solid wall
  
  for (i = 0; i < 200; i++)  // 200 times we're going to
  {
    int r = rand();  // pick a random number
    x1 = rr(1, SIZEX-20,r);  // make x1 between 1 and 108 - pick a position between 1 and 108 to draw a block of floor
    x2 = rr(x1+5, x1+19,r);  // make x2 between 6 and 127 - make this block of floor between 5 and 19 squares across
    y1 = rr(1, SIZEY-8,r);   // make y1 between 1 and 56 - pick a position between 1 and 56 to draw a block of floor
    y2 = rr(y1+3, y1+7,r);   // make y2 between 4 and 63 - make this block of floor between 3 and 7 squares down
    
    draw_region(x1, x2, y1, y2, ' ');  // draw blocks of floor at the regions the random number generator has made
  }
  
  rand_place('@', 5);  // place me somewhere, give me 5hp
  
  for (i = 0; i < 10; i++)  // 10 times
  {
    rand_place('m', 2);  // place monsters somewhere, give them 2hp
  }
}

static void draw_screen(void)  // function to draw the screen each turn
{
  int y;  // a counter representing each vertical line of the screen
  
  clear();  // from curses, clear the screen
  
  for (y = 0; y < SIZEY; y++)  // for each interation of the counter until the max number of lines (SIZEY)
  {
    mvaddstr(y, 0, map[y]);  // from curses, write the string which holds this line of the map array at line y
  }
}

static int sgn(int x)  // this is used in calculation to move the monster closer to the player, the "AI" so to speak
{
  return (x>0)-(x<0);  // if the monster's position is greater than the player's then decrease the monster position, and vice versa
}

int main(void)  // the main program
{
  int c;  // make a local integer to get numbers from the numpad
  int i = 0;  // this counter determines which actor we are moving. i am zero, monsters are 1 to MMAX
  srand(time(0));  // seed the random number generator with the current second
  init_map();  // generate the map. fill with wall, make blank spaces, place the player, place 10 monsters
  
  initscr();  // curses screen initialisation
  cbreak();  // makes characters typed immediately available to the progrma 
  
  while (1)  // do this loop forever
  {
    int nx = mx[i], ny = my[i];  // get the actor's current position and prepare it for modification
    if (!i)  // when i is zero it's my turn so we enter this code block, otherwise we're doing monster movement so we bypass it
    {
      draw_screen();  // draw the current state of the screen. use curses to add each line of the map array
      c = 0;  // clear this so i haven't pressed anything each loop
      
      while (!(c=getch()));  // wait for my input and place it in c
      
      if ((c == '1') || (c == '4') || (c == '7')) nx -= 1;  // if i've pushed a left column key, set next position left
      if ((c == '3') || (c == '6') || (c == '9')) nx += 1;  // if i've pushed a right column key, set next position right
      if ((c == '7') || (c == '8') || (c == '9')) ny -= 1;  // if i've pushed a top row key, set next position up
      if ((c == '1') || (c == '2') || (c == '3')) ny += 1;  // if i've pushed a bottom row key, set next position down
      
      if (map[ny][nx] == 'm')  // if there is a monster at the next position
      {
        int j = getm(nx, ny);  // get the number of the monster in the array
        
        mhp[j]--;  // take one away from the monster's hitpoints
        
        if (!mhp[j])  // if the monster is now dead
        {
          map[ny][nx] = ' ';  // draw floor at his position
          mc[j] = 0;  // set the monster's character to 0
        }
      }
    }
    
    if (mc[i] == 'm')  // if this actor's monster character entry contains a live monster
    {
      nx += sgn(mx[0]-nx);  // move the monster towards me
      ny += sgn(my[0]-ny);  // move the monster towards me
      
      if (map[ny][nx] == '@')  // if the monster has run into me
      {
        // mhp[0]--;  // take one away from my hitpoints - comment this out for god mode :P
        
        if (!mhp[0])  // if i am dead
        {
          endwin();  // end curses control of this window
          exit(0);  // successful program quit
        }
      }
    }
      
    if (mc[i] && (map[ny][nx] == ' '))  // if the current character is moving into blank space
    {
      map[my[i]][mx[i]] = ' ';  // draw the previous position as blank space in the map array
        
      mx[i] = nx;  // move into the new location
      my[i] = ny;  // move into the new location
        
      map[ny][nx] = mc[i];  // put the entity's character at the new location in the map array
    }
    
    i++;  // increment the actor counter
    
    if (i == MMAX) i = 0;  // if we've reached the maximum number of actors, we've moved all the monsters, so go back to waiting for player input
  }
  
  return(0); // successful program exit
  // however, though considering the game loop is a while true, and we just exit(0) on death, we never actually get here
}
