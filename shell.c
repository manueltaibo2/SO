shell.c

#include <stdio.h>
#include <string.h>

#include "commands.h"
#define MAX_INPUT 1024
#define MAX_TOKENS 256

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;

  if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
  return i;
}

void porcessInput (char* tokens[]){
  boll found = false;
  for(int i = 0; i< n_commands; i++){
    cmd_definition current = commands[i];
    if( !strcmp(tokens[0], current.cmd_name)){
      found = true;
      current.handler(tokens);
      break;
    }
  }
  if(!found){
    printf("command not found : %s\n", tokens[0];
  }
}

int main(){
  char buffer[MAX_INPUT]
  char* tokens[MAX_TOKENS]
  char* result;
  int num_tokens;
  
  bool finished = false;
  
  while (!finished){
    printf(">");
    result = fgets(buffer, MAX_INPUT,stdin);
    if(result == NULL){
     finished = true;
     }else {
      num_tokens = TrocearCadena(buffer
      proccesInput(tokens);
      }
      }
     
  
  return 0;
  }
