/*tinybc.c  * a tiny calculator that uses de to do its r•ork
            * demonstrates bidirectional pipes
            * input looks like number op number which
                tinybc converts into number \n number \n op \n p
                and passes result back to stdout
         +-----------+                +----------+
 stdin  >0           >== pipetodc ====>          |
         |   tinybc  |                |    dc    |
 stdout <1           <== pipefromdc ==<          |
         +-----------+                +----------+
    * program outline
        a. get two pipes
        b. fork [ get another process]
        c. in the de-to-be process,
            connect stdin and out to pipes
            then execl dc
        d. in the tinybc-pr。cess, no plumbing to do
            just talk to human via normal i/o
            and send stuff via pipe
        e . then close pipe and dc dies
    * note: does not handle multi line ansv•ers
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define oops(m,x) {perror(m); exit(x);}

void be_dc(int *, int *);
void be_bc(int * , int* );
void fatal(char *);

int main()
{
    int pid, todc[2], fromdc[2];

    // get two pipes
    if (pipe(todc) == -1 || pipe(fromdc) == -1)
        oops("pipe failed", 1);

    // get a process for user interface
    if ((pid = fork()) == -1)
        oops("cannot fork", 2);

    // be user interface
    if (pid == 0)
        be_bc(todc, fromdc);

    // be 'de
    else {
        be_dc(todc, fromdc);
        wait(NULL);
    }

    return 0;
}

void fatal(char *mess)
{
    fprintf(stderr, "Error: %s\n", mess);
    exit(1);
}

void be_dc(int in[2], int out[2])
{
    // setup stdin from pipein
    if (dup2(in[0], 0) == -1)
        oops("dc: cannot redirect stdin",3);
    close(in[0]);
    close(in[1]);

    // setup stdout to pipeout
    if (dup2(out[1], 1) == -1)
        oops("dc: cannot redirect stdout",4);
    close(out[1]);
    close(out[0]);

    // now execl dc with the - option
    execlp("dc", "dc", "-", NULL);
    oops("cannot run dc",5);
}

void be_bc(int todc[0], int fromdc[1])
{
    int num1, num2;
    char operation[BUFSIZ], message[BUFSIZ], *fgets();
    FILE *fpout, *fpin, *fdopen();

    // setup
    close(todc[0]);
    close(fromdc[1]);
    fpout = fdopen(todc[1], "w");
    fpin = fdopen(fromdc[0], "r");

    if (fpout == NULL || fpin == NULL)
        fatal("Error convering pipes to streams");

    // main loop
    while (printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL) {
        // parse input
        if (sscanf(message, "%d%[-+*/^]%d", &num1, operation, &num2) != 3) {
            printf("syntax error\n");
            continue;
        }

        if (fprintf(fpout, "%d\n%d\n%c\np\n", num1, num2, *operation) == EOF)
            fatal("Error writing");
        fflush(fpout);
        if (fgets(message, BUFSIZ, fpin) == NULL)
            break;
        printf("%d %c %d = %s", num1, *operation, num2, message);
    }
    fclose(fpout);
    fclose(fpin);
}