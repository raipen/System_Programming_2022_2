/* sigdemo.c - shows how a signal handler works
 * - run this and press Ctrl-C a few times
 */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int i = 0;

void f(int signum)
{
    printf("Currently elapsed time: %d sec(s)",i);
}

int main()
{
    void f(int);
    signal(SIGINT, f);
    printf("you can't stop me!\n");
    for (;;)
    {
        printf("haha\n");
        i++;
        sleep(1);
    }

    return 0;
}
