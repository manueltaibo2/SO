/*
Antón Vázquez López anton.vazquez.lopez@udc.es
Manuel Taibo González manuel.taibo2@udc.es
*/

#ifndef P0_H
#define P0_H

#include "list.h"

void processInput (char* tokens[]);
void cmd_authors (char* trozos[]);
void cmd_getpid (char* trozos[]);
void cmd_getcwd (char* trozos[]);
void cmd_chdir ( char* trozos[]);
void cmd_exit (char* trozos[]);
void cmd_date (char* trozos[]);
void cmd_hour(char *trozos[]);
void cmd_historic(char *trozos[]);
void cmd_open (char * trozos[]);
void cmd_close (char * trozos[]);
void cmd_dup(char *trozos[]);
void cmd_listopen(char *trozos[]);
void cmd_infosys(char *trozos[]);
void cmd_help(char *trozos[]);

#endif
