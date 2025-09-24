/*
Antón Vázquez López anton.vazquez.lopez@udc.es
Manuel Taibo González manuel.taibo2@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/utsname.h>

#include "list.h"
#include "p0.h"
#define MAX_INPUT 1024
#define MAX_TOKENS 256


//
typedef void (*cmd_handler)(char*[]);

typedef struct{
  char* cmd_name;
  cmd_handler handler;
} cmd_definition;

cmd_definition commands[] = {
  {"authors", cmd_authors},
  {"getpid", cmd_getpid},
  {"exit", cmd_exit},
  {"quit", cmd_exit},
  {"bye", cmd_exit},
  {"date", cmd_date},

};

int n_commands = sizeof(commands) / sizeof(cmd_definition);  //tambien vale sizeof(commands[0] pero igual accedes a algo que no existe;
//int n_commands = sizeof(commands) / sizeof(commands[0]);

void processInput (char* tokens[]){
  bool found = false;
  for(int i = 0; i< n_commands; i++){
    cmd_definition current = commands[i];
    if( !strcmp(tokens[0], current.cmd_name)){
      found = true;
      current.handler(tokens);
      break;
    }
  }
  if(!found){
    printf("command not found : %s\n", tokens[0]);
  }
}

void cmd_authors (char* trozos[]){
  //print("Authors ejecutado\n");
  if (trozos[1] == NULL){
  printf("Antón Vázquez López anton.vazquez.lopez@udc.es\nManuel Taibo González manuel.taibo2@udc.es\n");
    }else if (strcmp(trozos[1], "-l") == 0){
      printf("Logins: anton.vazquez.lopez@udc.es, manuel.taibo2@udc.es\n");
    }else if(strcmp(trozos[1], "-n") == 0){
      printf("Nombres: Antón Vázquez López, Manuel Taibo González\n");
    } else{
      printf("Opción no válida para 'authors'.\n");
    }
  }
  
void cmd_getpid ( char* trozos[]){
  printf("Mi PID es : me lo invento\n");
}
  
void cmd_date (char* trozos[]){
    time_t aux;
    struct tm *Fjunta;
    char fecha[11];
    char hora [9];

    time(&aux);
    Fjunta = localtime(&aux);

    strftime(fecha, sizeof(fecha), "%d/%m/%Y", Fjunta);
    strftime(hora, sizeof(hora), "%H:%M:%S", Fjunta);
    
    if (trozos[1] == NULL){
        printf("Fecha: %s\n", fecha);
    printf("Hora: %s\n", hora);
    } else if(strcmp(trozos[1], "-d") == 0){
    printf("Fecha: %s\n", fecha);
    } else if(strcmp(trozos[1], "-t") == 0){
    printf("Hora: %s\n", hora);
    }
}

void cmd_exit(char* tr[]){
  exit(0);
  }



int main(){
  char buffer[MAX_INPUT];
  char* tokens[MAX_TOKENS];
  char* result;
  int num_tokens;

  bool finished = false;

  while (!finished){
    printf(">");
    result = fgets(buffer, MAX_INPUT,stdin);
    if(result == NULL){
      finished = true;
    }else {
      num_tokens = TrocearCadena(buffer, tokens);
      processInput(tokens);
      }
  }


  return 0;
}
