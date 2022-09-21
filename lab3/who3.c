/** who3.c -read /etc/utmp and list info therein
 *  -suppresses empty records
 *  -formats time nicely
 *  -buffers input (using utmplib)
 */
#include <stdio.h>
#include <sys/types.h>
#include <utmp.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#define SHOWHOST

int utmp_open(char *);
void utmp_close();
void showtime(long);
void show_info(struct utmp *);

int main()
{
    struct utmp *utbufp, *utmp_next();
    if (utmp_open(UTMP_FILE) == -1) {
        perror(UTMP_FILE); //UTMP_FILE is in utmp.h
        exit(1);
    }
    while ((utbufp = utmp_next()) != ((struct utmp *) NULL))
        show_info(utbufp);
    utmp_close();
    return 0; //went ok
}

void show_info(struct utmp *utbufp)
{
    if (utbufp->ut_type != USER_PROCESS)
        return;
    printf("%-8.8s", utbufp->ut_name); //the logname
    printf(" "); //a space
    printf("%-8.8s", utbufp->ut_line); //the tty
    printf(" "); //a space
    showtime(utbufp->ut_time);
#ifdef SHOWHOST
    if (utbufp->ut_host[0] != '\0')
        printf("(%s)", utbufp->ut_host); //the host
#endif
    printf("\n"); //final end of line
}

void showtime(long timeval){
    char *cp;
    cp = ctime(&timeval); //convert time to string
    printf("%12.12s", cp+4); //pick 12 chars from pos 4
}
