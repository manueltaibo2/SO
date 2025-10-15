#ifndef COMANDOS_H
#define COMANDOS_H

#include "list.h"

void cmd_authors(char*[]);
void cmd_getpid(char*[]);
void cmd_getcwd(char*[]);
void cmd_chdir(char*[]);
void cmd_date(char*[]);
void cmd_hour(char*[]);
void cmd_historic(char*[]);
void cmd_open(char*[]);
void cmd_close(char*[]);
void cmd_dup(char*[]);
void cmd_listopen(char*[]);
void cmd_infosys(char*[]);
void cmd_help(char*[]);
void cmd_exit(char*[]);

//P1
void cmd_create(char*[]);
void cmd_erase(char*[]);
void cmd_delrec(char*[]);
void cmd_lseek(char*[]);
void cmd_writestr(char*[]);

#endif