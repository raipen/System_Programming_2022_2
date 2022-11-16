#include <stdio.h>
#include <curses.h>
#include <unistd.h>
// display a blinking message
// Use the sleep function to pause the program between printing the message and then erasing it

#define MSG     "Hello, world"
#define BLANK   "            "

int main()
{
    initscr();
    clear();
    while (1)
    {
        move(10, 20);
        addstr(MSG);
        refresh();
        sleep(1);
        move(10, 20);
        addstr(BLANK);
        refresh();
        sleep(1);
    }
    endwin();
    return 0;
}