/* stdinredir2.c
* shows two more methods for redirecting standard input
* use #define to set on or the other
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define CLOSE_DUP
//#define CLOSE_DUP2

int main()
{
    int fd;
    int newfd;
    char line[100];

    // read and print three lines
    fgets(line, 100, stdin);
    printf("%s", line);
    fgets(line, 100, stdin);
    printf("%s", line);
    fgets(line, 100, stdin);
    printf("%s", line);

    // close fd 0
    fd = open("/etc/passwd", O_RDONLY);
#ifdef CLOSE_DUP
    close(0);
    newfd = dup(fd);
#else
    newfd = dup2(fd, 0);
#endif

    // open a file and check for errors
    if (newfd != 0)
    {
        fprintf(stderr, "Could not open duplicate fd to 0\n");
        exit(1);
    }
    close(fd);

    // read and print three lines
    fgets(line, 100, stdin);
    printf("%s", line);
    fgets(line, 100, stdin);
    printf("%s", line);
    fgets(line, 100, stdin);
    printf("%s", line);

    return 0;
}