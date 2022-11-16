/* process.c
 * command processing layer
 *
 * The process(char **arglist) function is called by the main loop
 * It sits in fromt of the execute() function, This layer handles
 * thw main classes of processing:
 *     a) built-in functions (cd, exit(), set, =, read, ...)
 *     b) control structures (if, while, for, ...)
 */

#include <stdio.h>
#include "smsh.h"

int is_control_command(char *);
int do_control_command(char **);
int ok_to_execute();
int builtin_command(char **, int *);

int process(char **arglist)
/*
 * purpose: process a single command
 * returns: result of processing command
 * errors: -1 on fork() or wait() errors
 * notes: calls fork, but not exec
 */
{
    int rv = 0;

    if (arglist[0] == NULL)
        rv = 0;
    else if (is_control_command(arglist[0]))
        rv = do_control_command(arglist);
    else if (ok_to_execute())
        if(!builtin_command(arglist, &rv))
            rv = execute(arglist);

    return rv;
}