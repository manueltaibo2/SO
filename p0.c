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

void cmd_authors (char* tr[]){
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
  
void cmd_getpid( char* tr[]){
  print("Mi PID es : me lo invento\n");
  }
  
void date (char* tr[]){
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
  
void cmd_
