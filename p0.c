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
  {"getcwd", cmd_getcwd},
  {"chdir", cmd_chdir},
  {"exit", cmd_exit},
  {"quit", cmd_exit},
  {"bye", cmd_exit},
  {"date", cmd_date},
  {"hour", cmd_hour},
  {"historic",(cmd_handler)cmd_historic},
  {"open", (cmd_handler)cmd_open},      //REVISAR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  {"close", (cmd_handler)cmd_close},
  {"dup", (cmd_handler)cmd_dup},
  {"listopen", (cmd_handler)cmd_listopen},
  {"infosys", cmd_infosys},
  {"help",cmd_help}


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


//funciones P0
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
  if (trozos[1] == NULL){
    pid_t pid = getpid();
    printf("Identificador proceso: %d\n", pid);
  } else if (strcmp(trozos[1], "-p") == 0){
    pid_t pid = getppid();
    printf("Identificador proceso padre: %d\n", pid);
  } else {
    printf("Opción no válida para 'getpid'.\n");
  }
}

void cmd_getcwd ( char* trozos[]){
  char dir[MAX_TOKENS];

  if (trozos[1] == NULL){
    getcwd(dir, sizeof(dir));
    printf("Directorio actual: %s\n", dir);
  } else {
    printf("Opción no válida para 'getcwd'.\n");
  }
}

void cmd_chdir ( char* trozos[]){
  //char dir[MAX_TOKENS];

  if (trozos[1] == NULL){
    cmd_getcwd(trozos);
  } else if (trozos[1] != NULL) {
    chdir(trozos[1]);
    printf("Se ha cambiado el directorio actual a: %s\n", trozos[1]);
  } else {
    printf("Opción no válida para 'chdir'.\n");
  }
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

void cmd_hour(char *trozos[]){
  time_t now = time(NULL);
  struct tm *Fjunta = localtime(&now);
  char hora[9];

  strftime(hora, sizeof(hora), "%H:%M:%S", Fjunta);
  printf("Hora: %s\n", hora);
}

void cmd_historic(Lista *lista, char *trozos[]) {
    if (trozos[1] == NULL) {
        // Caso: historic -> imprimir todos
        ImprimirComandos(*lista);
    } else if (strcmp(trozos[1], "-count") == 0) {
        printf("Número de comandos en histórico: %d\n", lista->tamaño);
    } else if (strcmp(trozos[1], "-clear") == 0) {
        LiberarLista(lista);
        printf("Histórico borrado.\n");
    } else {
        int n = atoi(trozos[1]);
        if (n >= 0) {
            // historic N -> repetir el comando número N
            ImprimirComandoN(*lista, n);
        } else {
            // historic -N -> imprimir últimos N comandos
            ImprimirComandoMN(*lista, -n);
        }
    }
}

void cmd_open(char * trozos[], ListaArchivos *lista){
  int mode=0, df;

  if (trozos[1] == NULL){
    ListarFicherosAbiertos(lista);
    return;
  }

  for (int i = 2; trozos[i] != NULL; i++){
    if (!strcmp(trozos[i], "cr")) mode |= O_CREAT;
    else if (!strcmp(trozos[i], "ex")) mode |= O_EXCL;
    else if (!strcmp(trozos[i], "ro")) mode |= O_RDONLY;
    else if (!strcmp(trozos[i], "wo")) mode |= O_WRONLY;
    else if (!strcmp(trozos[i], "rw")) mode |= O_RDWR;
    else if (!strcmp(trozos[i], "ap")) mode |= O_APPEND;
    else if (!strcmp(trozos[i], "tr")) mode |= O_TRUNC;
    else break;
  }

  df = open(trozos[1], mode, 0666);
  if (df == -1){
    printf("Error al abrir el archivo\n");
    return;
  }

  AgregarArchivo(lista, trozos[1], df, mode);
  printf("Archivo abierto: %s, Descriptor: %d\n", trozos[1], df);

}

void cmd_close(char *trozos[], ListaArchivos *lista){
  int df = atoi(trozos[1]);

  if (trozos[1] == NULL || df<0){
    ListarFicherosAbiertos(lista);
    return;
  }

  if (close(df) == -1) printf("Error al cerrar el archivo\n");
  else {
    EliminarDeFicherosAbiertos(lista, df);
    printf("Archivo con descriptor %d cerrado correctamente\n", df);
  }
}

void cmd_dup(char *trozos[], ListaArchivos *lista) {
  int df, copia;
  char *nombre;
  char aux[MAX_TOKENS];

  if (trozos[1] == NULL || (df=atoi(trozos[1]))<0){
    ListarFicherosAbiertos(lista);
    return;
  }

  copia = dup(df);
  if (copia == -1){
    printf("Error al duplicar el archivo\n");
    return;
  }

  nombre = NombreFicheroDescriptor(df, lista);
  if (nombre == NULL){
    printf("No se encontró el archivo con descriptor: %d\n", df);
    close(copia);
    return;
  }

  sprintf(aux, "dup %d (%s)", df, nombre);
  AgregarArchivo(lista, aux, copia, 0);
  printf("Archivo con descriptor: %d  duplicado correctamente\n", copia);
}

void cmd_listopen(ListaArchivos *lista){
  if(lista==NULL) return;
  ListarFicherosAbiertos(lista);
}

void cmd_infosys(){
  struct utsname info;
  if (uname(&info) == -1){
    printf("Error al obtener información del sistema\n");
    return;
  }

  printf("System info:\n");
  printf("Sistema operativo: %s\n", info.sysname);
  printf("Nombre del equipo: %s\n", info.nodename);
  printf("Versión del sistema: %s\n", info.release);
  printf("Versión del kernel: %s\n", info.version);
  printf("Arquitectura: %s\n", info.machine);
}

void cmd_help(char *trozos[]){
    if (trozos[1] == NULL){
        puts("COMANDOS:");
        puts("  - authors: Muestra autores del programa.");
        puts("  - getpid: Muestra el PID del proceso actual.");
        puts("  - chdir: Cambia de directorio.");
        puts("  - getcwd: Muestra directorio actual.");
        puts("  - date: Muestra la fecha y hora.");
        puts("	- hour: Muestra la hora en formato hh:mm:ss.");
        puts("  - historic: Muestra el historial de comandos.");
        puts("  - open: Abre un archivo.");
        puts("  - close: Cierra un archivo.");
        puts("  - dup: Duplica un archivo.");
        puts("  - listopen: Muestra todos los archivos abiertos");
        puts("  - infosys: Muestra información del sistema");
        puts("  - help: Muestra lista y descripción de comandos disponibles");
        puts("  - exit: Sale del shell");
    } else if(strcmp(trozos[1], "authors") == 0){
        printf("AUTHORS:  Muestra autores del programa\n  - USO: authors [-l | -n]\n");
    } else if(strcmp(trozos[1], "getpid") == 0){
        printf("GETPID:  Muestra el PID del proceso actual.\n  - USO: getpid [-p]\n");
    } else if(strcmp(trozos[1], "chdir") == 0){
        printf("CHDIR:  Cambia de directorio.\n  - USO: chdir <directorio>\n");
    } else if(strcmp(trozos[1], "getcwd") == 0){
        printf("GETCWD:  Muestra directorio actual.\n  - USO: getcwd\n");
    } else if(strcmp(trozos[1], "date") == 0){
        printf("DATE:  Muestra la fecha y hora.\n  - USO: date [-d | -t]\n");
    } else if(strcmp(trozos[1], "hours") == 0){
        printf("HOUR:  Muestra la hora en formato hh:mm:ss.\n  - USO: hour\n");
    } else if(strcmp(trozos[1], "historic") == 0){
        printf("HISTORIC:  Muestra el historial de comandos.\n  - USO: historic [-N | -n | -clear | - count]\n");
    } else if(strcmp(trozos[1], "open") == 0){
        printf("OPEN:  Abre un archivo.\n  - USO: open <archivo> [modos]\n  - MODOS: cr, ap, ex, ro, rw, wo, tr\n");
    } else if(strcmp(trozos[1], "close") == 0){
        printf("CLOSE:  Cierra un archivo.\n  - USO: close <descriptor>\n");
    } else if(strcmp(trozos[1], "dup") == 0){
        printf("DUP:  Duplica un descriptor de archivo.\n  - USO: dup <descriptor>\n");
    } else if(strcmp(trozos[1], "listopen") == 0){
        printf("LISTOPEN:  Muestra todos los archivos abiertos.\n  - USO: listopen\n");
    } else if(strcmp(trozos[1], "infosys") == 0){
        printf("INFOSYS:  Muestra información del sistema.\n  - USO: infosys\n");
    } else if(strcmp(trozos[1], "help") == 0){
        printf("HELP:  Muestra lista y descripción de comandos disponibles.\n  - USO: help [comando]\n");
    } else if(strcmp(trozos[1], "exit") == 0){
        printf("EXIT:  Sale del shell.\n  - USO: exit | quit | bye\n");
    } else {
        printf("Comando no encontrado: %s\n", trozos[1]);
    }
}

void cmd_exit(char* tr[]){
  exit(0);
  }

int main(){
  char buffer[MAX_INPUT];
  char* tokens[MAX_TOKENS];
  char* result;

  Lista listaComandos;
  ListaArchivos listaArchivos;

  InicializarLista(&listaComandos);
  InicializarListaArchivos(&listaArchivos);

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
        if (strcmp(tokens[0], "historic") == 0) {
          cmd_historic(&listaComandos, tokens);
        } else if (strcmp(tokens[0], "open") == 0) {
          cmd_open(tokens, &listaArchivos);
        } else if (strcmp(tokens[0], "close") == 0) {
          cmd_close(tokens, &listaArchivos);
        } else if (strcmp(tokens[0], "dup") == 0) {
          cmd_dup(tokens, &listaArchivos);
        } else if (strcmp(tokens[0], "listopen") == 0) {
          cmd_listopen(&listaArchivos);
        } else {
          // Comandos normales que no necesitan listaArchivos
          processInput(tokens);
        }
      }
    }
  }
  LiberarLista(&listaComandos);
  LiberarListaArchivos(&listaArchivos);
  return 0;
}
