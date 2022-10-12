/* write0.c
    * purpose: send messages to another terminal
    * method: open the other terminal for output then copy from stdin to that terminal
    * shows: a terminal is just a file supporting regular i/o
    * usage: write0 ttyname
    */

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define oops(m,x) {perror(m); exit(x);}

int main(int ac, char *av[])
{
    int fd;
    char buf[BUFSIZ];

    if (ac != 2) {
        fprintf(stderr, "usage: write0 ttyname\n");
        exit(1);
    }

    fd = open(av[1], O_WRONLY);
    if (fd == -1)
        oops(av[1], 2);

    while (fgets(buf, BUFSIZ, stdin) != NULL)
        if (write(fd, buf, strlen(buf)) == -1)
            break;

    close(fd);

    return 0;
}