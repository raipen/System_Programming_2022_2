/* more01.c -version 0.1 of more
* read and print 24 lines then pause for a few special commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

static int page_len;
#define LINELEN 512

void do_more(FILE *);
int see_more();

int main(int ac, char *av[])
{
    struct winsize wbuf;
    ioctl(0, TIOCGWINSZ, &wbuf);
    page_len = wbuf.ws_row -1; // -1 for the prompt line
    FILE *fp;
    if (ac == 1)
        do_more(stdin);
    else
        while (--ac)
            if ((fp = fopen(*++av, "r")) != NULL) {
                do_more(fp);
                fclose(fp);
            } else
                exit(1);
    return 0;
}

void do_more(FILE *fp)
/*
* read PAGELEN lines, then call see_more() for further instructions
*/
{
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(), reply;
    while (fgets(line, LINELEN, fp)) { /* more input */
        if (num_of_lines == page_len) { 
            reply = see_more();
            if (reply == 0) /* n: done */
                break;
            num_of_lines -= reply; /* reset count */
        }
        if (fputs(line, stdout) == EOF) /* show line */
            exit(1); /* or die */
        num_of_lines++; /* count it */
    }
}

int see_more()
/*
* print message, wait for response, return # of lines to advance
* q means no, space means yes, CR means one line
*/
{
    int c;
    printf("\033[7m more? \033[m"); /* reverse on a vt100 */
    while ((c = getchar()) != EOF) { /* NEW: reads from tty */
        if (c == 'q') /* q -> N */
            return 0;
        if (c == ' ') /* ' ' => next page */
            return page_len; /* how many to show */
        if (c == '\n') /* Enter key => 1 line */
            return 1;
    }
    return 0;
}
