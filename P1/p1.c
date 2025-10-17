/*
Antón Vázquez López anton.vazquez.lopez@udc.es
Manuel Taibo González manuel.taibo2@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "list.h"
#include "p1.h"
#include "comandos.h"

#define MAX_INPUT 1024
#define MAX_TOKENS 256


//
typedef void (*cmd_handler)(char*[]);

Lista listaComandos;
ListaArchivos listaArchivos;


typedef struct{
  char* cmd_name;
  cmd_handler handler;
} cmd_definition;

cmd_definition commands[] = {
  {"authors", cmd_authors},
  {"getpid", cmd_getpid},
  {"getcwd", cmd_getcwd},
  {"chdir", cmd_chdir},
  {"exit", cmd_exit},
  {"quit", cmd_exit},
  {"bye", cmd_exit},
  {"date", cmd_date},
  {"hour", cmd_hour},
  {"historic", cmd_historic},
  {"open", cmd_open},      
  {"close", cmd_close},
  {"dup", cmd_dup},
  {"listopen", cmd_listopen},
  {"infosys", cmd_infosys},
  {"help",cmd_help},
  {"create", cmd_create},
  {"erase", cmd_erase},
  {"delrec", cmd_delrec},
  {"lseek", cmd_lseek},
  {"writestr", cmd_writestr}
  
};

int n_commands = sizeof(commands) / sizeof(cmd_definition);  


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


int main(){
  char buffer[MAX_INPUT];
  char* tokens[MAX_TOKENS];
  char* result;


  InicializarLista(&listaComandos);
  InicializarListaArchivos(&listaArchivos);

  // Agregar descriptores estándar con sus modos reales
  int modo_stdin = fcntl(0, F_GETFL);
  int modo_stdout = fcntl(1, F_GETFL);
  int modo_stderr = fcntl(2, F_GETFL);

  AgregarArchivo(&listaArchivos, "stdin", 0, modo_stdin);
  AgregarArchivo(&listaArchivos, "stdout", 1, modo_stdout);
  AgregarArchivo(&listaArchivos, "stderr", 2, modo_stderr);

  bool finished = false;

  while (!finished){
    printf(">");
    result = fgets(buffer, MAX_INPUT,stdin);
    if(result == NULL){
      finished = true;
    } else {
      AgregarComando(&listaComandos, buffer);
      TrocearCadena(buffer, tokens);

      // Verificar qué tipo de comando es
      if (tokens[0] != NULL) {
          // Comandos normales que no necesitan listaArchivos
          processInput(tokens);
        }
      }
    }

  LiberarLista(&listaComandos);
  LiberarListaArchivos(&listaArchivos);
  return 0;
}