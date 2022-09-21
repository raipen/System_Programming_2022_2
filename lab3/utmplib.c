/* utmplib.c - functions to buffer reads from utmp file
 * 
 *  functions are
 *      utmp_open(filename) - open file
 *          return -1 on error
 *      utmp_next() - return pointer to next struct
 *          return NULL on eof
 *      utmp_close() - close file
 * 
 *  reads NRECS per read and then doles them out from the buffer
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <utmp.h>
#include <unistd.h>

#define NRECS 16
#define NULLUT ((struct utmp *)NULL)
#define UTSIZE (sizeof(struct utmp))

static char utmpbuf[NRECS * UTSIZE]; //storage
static int num_recs; //num stored
static int cur_rec; //next to go
static int fd_utmp = -1; //read from

int utmp_reload();

int utmp_open(char *filename)
{
    fd_utmp = open(filename, O_RDONLY); //open it
    cur_rec = num_recs = 0; //no recs yet
    return fd_utmp;
}

struct utmp *utmp_next()
{
    struct utmp *recp;
    if (fd_utmp == -1) //error?
        return NULLUT;
    if (cur_rec == num_recs && utmp_reload() == 0) //any more?
        return NULLUT;
    //get address of next record
    recp = (struct utmp *)&utmpbuf[cur_rec * UTSIZE];
    cur_rec++;
    return recp;
}

int utmp_reload()
{
    int amt_read; //how many did we get?
    amt_read = read(fd_utmp, utmpbuf, NRECS * UTSIZE); //read them in
    num_recs = amt_read/UTSIZE; //reset pointer
    cur_rec = 0;
    return num_recs;
}

void utmp_close()
{
    if (fd_utmp != -1)
        close(fd_utmp);
}