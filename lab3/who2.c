//read /etc/utmp and list info therein
//suppresses empty records
//formats time nicely
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#define SHOWHOST
void showtime(long);
void show_info(struct utmp *);

int main()
{
    struct utmp current_record; //read info into here
    int utmpfd; //read from this descriptor
    int reclen = sizeof(current_record);

    if ((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE); //UTMP_FILE is in utmp.h
        exit(1);
    }

    while (read(utmpfd, &current_record, reclen) == reclen)
        show_info(&current_record);
    close(utmpfd);
    return 0; //went ok
}

//show info()
//displays contents of the utmp struct in human readable form
// *note* these sizes should not be hardwired

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