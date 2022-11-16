/* splitline.c - command reading and parsing functions for smsh
 *
 * char *next_cmd(char *prompt, FILE *fp) - get next command
 * char **splitline(char *str) - parse a string
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smsh.h"

int isNewLine = 1;

char *next_cmd(char *prompt, FILE *fp)
/*
 * purpose: read next command from fp
 * returns: dynamically allocated string holding command
 * errors: NULL at EOF (not really an error)
 * notes: allocates space in BUFSIZ chunks.  
 */
{
    char *buf;          /* the buffer */
    int bufspace = 0;   /* total size */
    int pos = 0;        /* current position */
    int c;              /* input char */

    if(isNewLine)
        printf("%s", prompt);       /* prompt user */
    while ((c = getc(fp)) != EOF) {

        /* need space? */
        if (pos+1 >= bufspace) {    /* 1 for \0 */
            if (bufspace == 0)      /* y: 1st time */
                buf = emalloc(BUFSIZ);
            else                    /* or expand */
                buf = erealloc(buf, bufspace+BUFSIZ);
            bufspace += BUFSIZ;     /* update size */
        }

        /* end of command? */
        if (c == '\n'||c==';'){
            isNewLine = c == '\n' ? 1 : 0;
            break;
        }

        /* no, add to buffer */
        buf[pos++] = c;
    }
    if(c == EOF && pos == 0)    /* EOF and no input */
        return NULL;
    buf[pos] = '\0';
    return buf;
}

#define is_delim(x) ((x)==' '||(x)=='\t')

char **splitline(char *line)
/*
 * purpose: split a line into array of white-space separated tokens
 * returns: a NULL-terminated array of pointers to copies of the tokens
 *  or NULL if line if no tokens on the line
 * action: traverse the array, locate strings, make copies
 * note: strtok() could work, but we may want to add quotes later
 * errors: memory allocation
 *  returns NULL if line is NULL
 */
{
    char *newstr();
    char **args;
    int spots = 0;      /* spots in table */
    int bufspace = 0;   /* bytes in table */
    int argnum = 0;     /* slots used */
    char *cp = line;    /* pos in string */
    char *start;
    int len;

    if (line == NULL)   /* handle special case */
        return NULL;

    args = emalloc(BUFSIZ); /* initialize array */
    bufspace = BUFSIZ;
    spots = BUFSIZ/sizeof(char *);

    while (*cp != '\0') {
        while (is_delim(*cp))   /* skip leading spaces */
            cp++;
        if (*cp == '\0')        /* end of string? */
            break;              /* yes, get out */

        /* make sure the array has room (+1 for NULL) */
        if (argnum+1 >= spots) {
            args = erealloc(args, bufspace+BUFSIZ);
            bufspace += BUFSIZ;
            spots += (BUFSIZ/sizeof(char *));
        }

        /* make start point to beginning of next word */
        start = cp;
        len = 1;
        while (*++cp != '\0' && !(is_delim(*cp))) /* find end of word */
            len++;
        args[argnum++] = newstr(start, len);
    }
    args[argnum] = NULL;

    return args;
}

char *newstr(char *s, int l)
/*
 * purpose: constructor for strings
 * returns: a string, never NULL
 */
{
    char *rv = emalloc(l+1);

    rv[l] = '\0';
    strncpy(rv, s, l);
    return rv;
}

void freelist(char **list)
/*
 * purpose: free the list returned by splitline
 * returns: nothing
 * action: free all strings in list and then free the list
 */
{
    char **cp = list;
    while (*cp)
        free(*cp++);
    free(list);
}

void *emalloc(size_t n)
/*
 * purpose: wrapper for malloc
 * returns: pointer to memory or dies
 */
{
    void *rv;
    if ((rv = malloc(n)) == NULL)
        fatal("out of memory", "", 1);
    return rv;
}

void *erealloc(void *p, size_t n)
/*
 * purpose: wrapper for realloc
 * returns: pointer to memory or dies
 */
{
    void *rv;
    if ((rv = realloc(p, n)) == NULL)
        fatal("realloc() failed", "", 1);
    return rv;
}