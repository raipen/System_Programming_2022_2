/* controlflow.c
 *
 * "if" processing is done with two state variables
 *      if_state and if_result
 */
#include <stdio.h>
#include <string.h>
#include "smsh.h"

enum states {NEUTRAL, WANT_THEN, THEN_BLOCK};
enum results {SUCCESS, FAIL};

static int if_state = NEUTRAL;
static int if_result = SUCCESS;
static int last_stat = 0;

int syn_err(char *);

int ok_to_execute()
/*
 * purpose: determine if we can execute this command
 * returns: 1 for yes, 0 for no
 */
{
    int rv = 1;

    if (if_state == WANT_THEN) {
        syn_err("then expected");
        rv = 0;
    } else if (if_state == THEN_BLOCK && if_result == SUCCESS)
        rv = 1;
    else if (if_state == THEN_BLOCK && if_result == FAIL)
        rv = 0;
    return rv;
}

int is_control_command(char *s)
/*
 * purpose: do simple parsing to see if this is a control command
 * returns: 0 if s is NOT a control command, otherwise the number
 *          of the command
 */
{
    return (strcmp(s, "if") == 0 || strcmp(s, "then") == 0 ||
            strcmp(s, "fi") == 0);
}

int syn_err(char *s)
/*
 * purpose: handle syntax error
 * returns: -1 to indicate error
 */
{   
    if_state = NEUTRAL;
    fprintf(stderr, "syntax error: %s\n", s);
    return -1;
}

int do_control_command(char **args)
/*
 * purpose: process control commands
 * returns: result of processing, or 0 if no command
 * errors: -1 on syntax error, 0 if not a command
 */
{
    char *cmd = args[0];
    int rv = -1;

    if (strcmp(cmd, "if") == 0) {
        if (if_state != NEUTRAL)
            rv = syn_err("if unexpected");
        else {
            last_stat = process(args + 1);
            if_result = (last_stat == 0 ? SUCCESS : FAIL);
            if_state = WANT_THEN;
            rv = 0;
        }
    } else if (strcmp(cmd, "then") == 0) {
        if (if_state != WANT_THEN)
            rv = syn_err("then unexpected");
        else {
            if_state = THEN_BLOCK;
            rv = 0;
        }
    } else if (strcmp(cmd, "fi") == 0) {
        if (if_state != THEN_BLOCK)
            rv = syn_err("fi unexpected");
        else {
            if_state = NEUTRAL;
            rv = 0;
        }
    } else
        fatal("internal error processing:", cmd, 2);
    return rv;
}