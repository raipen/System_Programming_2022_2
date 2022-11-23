/*Redirect the input of the sort command so it reads from a file
• The filename is specified on the command line*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int ac, char **av){
    int pid;
    int fd;

    if(ac != 2){
        fprintf(stderr, "usage: %s filename\n", *av);
        exit(1);
    }

    printf("About to run sort with input from %s\n", av[1]);

    if((pid = fork()) == -1){
        perror("fork");
        exit(1);
    }

    if(pid == 0){
        close(0);
        fd = open(av[1], O_RDONLY);
        execlp("sort", "sort", NULL);
        perror("execlp");
        exit(1);
    }

    if(pid != 0){
        wait(NULL);
        printf("Done running sort. < %s\n", av[1]);
    }

    return 0;
}