

#include "p0.h"
typedef void (*cmd_handler)(char*[]);

typedef struct{
  char* cmd_name;
  cmd_handler handler;
  } cmd_definition
  
  cmd_definition commands[] = {
    {"authors", cmd_authors}
    {"getpid", cmd_getpid}
    {"exit",cmd_exit}
    {"quit",cmd_exit}
    {"bye",cmd_exit}
    };
    
    int n_commands = sizeof(commands) / sizeof(commands[cmd_definition] ;//tambien vale sizeof(commands[0] pero igual accedes a algo que no existe;
