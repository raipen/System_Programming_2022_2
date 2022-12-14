#include <unistd.h>
#include <curses.h>

#define LEFTDGE 10
#define RIGHTDGE 30
#define ROW 10

int main()
{
   char *message = "Hello";
   char *blank = "     ";
   int dir = +1;
   int pos = LEFTDGE;

   initscr();
   clear();

   while (1)
   {
      move(ROW, pos);
      addstr(message);
      move(LINES - 1, COLS - 1);
      refresh();
      sleep(1);
      move(ROW, pos);
      addstr(blank);
      pos += dir;
      if (pos >= RIGHTDGE)
         dir = -1;
      if (pos <= LEFTDGE)
         dir = +1;
   }
   return 0;
}