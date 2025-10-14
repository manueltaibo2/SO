#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/utsname.h>

#include "list.h"
#include "comandos.h"

#define MAX_TOKENS 256

extern Lista listaComandos;
extern ListaArchivos listaArchivos;

void cmd_authors(char* trozos[]) {
    if (trozos[1] == NULL) {
        printf("Antón Vázquez López anton.vazquez.lopez@udc.es\n");
        printf("Manuel Taibo González manuel.taibo2@udc.es\n");
    } else if (strcmp(trozos[1], "-l") == 0) {
        if (trozos[2] != NULL) {
            printf("Opción no válida para 'authors'.\n");
            return;
        }
        printf("Logins: anton.vazquez.lopez@udc.es, manuel.taibo2@udc.es\n");
    } else if (strcmp(trozos[1], "-n") == 0) {
        if (trozos[2] != NULL) {
            printf("Opción no válida para 'authors'.\n");
            return;
        }
        printf("Nombres: Antón Vázquez López, Manuel Taibo González\n");
    } else {
        printf("Opción no válida para 'authors'.\n");
    }
}

void cmd_getpid ( char* trozos[]){
  if (trozos[1] == NULL){
    pid_t pid = getpid();
    printf("Identificador proceso: %d\n", pid);
  } else if (strcmp(trozos[1], "-p") == 0){
        if (trozos[2] != NULL) { // Si hay más argumentos, error
            printf("Opción no válida para 'getpid'.\n");
            return;
        }
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

void cmd_chdir(char* trozos[]) {
    if (trozos[1] == NULL) {
        // Sin argumentos → mostrar cwd actual
        cmd_getcwd(trozos);
    } else {
        // Intentar cambiar de directorio
        if (chdir(trozos[1]) == 0) {
            printf("Se ha cambiado el directorio actual a: %s\n", trozos[1]);
        } else {
            perror("chdir"); // Muestra el error del sistema
        }
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
    } else {
        printf("Opcion no valida para 'date'.\n");
        }
}

void cmd_hour(char *trozos[]){
  time_t now = time(NULL);
  struct tm *Fjunta = localtime(&now);
  char hora[9];

  strftime(hora, sizeof(hora), "%H:%M:%S", Fjunta);
  if (trozos[1] == NULL) {
        printf("Hora: %s\n", hora);
    } else {
        printf("Opción no válida para 'hour'.\n");
    }
}

void cmd_historic(char *trozos[]) {
    if (trozos[1] == NULL) {
        // Caso: historic -> imprime todos
        ImprimirComandos(listaComandos);
        return;
    }
    // caso -count
    if (strcmp(trozos[1], "-count") == 0) {
        printf("Número de comandos en histórico: %d\n", listaComandos.tamaño);
        return;
    }
    // caso -clear
    if (strcmp(trozos[1], "-clear") == 0) {
        LiberarLista(&listaComandos);
        printf("Histórico borrado.\n");
        return;
    }
    // convertimos a número
    char *endptr;
    int aux = strtol(trozos[1], &endptr, 10);

    if (*endptr != '\0') {
        // No era un número válido
        printf("Opción no válida para 'historic': %s\n", trozos[1]);
        return;
    }
    if (aux > 0) {
        // caso historic N 
        if (aux > listaComandos.tamaño) {
            printf("No existe el comando con número %d\n", aux);
        } else {
            ImprimirComandoN(listaComandos, aux - 1); // -1 porque la lista empieza en 0
        }
    } else if (aux < 0) {
        // caso historic -N 
        aux = abs(aux);
        if (aux > listaComandos.tamaño) {
            printf("No se han escrito %d comandos\n", aux);
        } else {
            ImprimirComandoMN(listaComandos, aux);
        }
    } else {
        printf("Opción no válida para 'historic': %s\n", trozos[1]);
    }
}
void cmd_open(char *trozos[]) {
    int mode = 0, df;

    if (trozos[1] == NULL) {
        ListarFicherosAbiertos(&listaArchivos);
        return;
    }

    if (trozos[2] == NULL) {
        printf("Error: falta el modo de apertura (cr, ex, ro, wo, rw, ap, tr)\n");
        return;
    }

    if (!strcmp(trozos[2], "cr")) mode = O_CREAT;  
    else if (!strcmp(trozos[2], "ex")) mode = O_EXCL; 
    else if (!strcmp(trozos[2], "ro")) mode = O_RDONLY;
    else if (!strcmp(trozos[2], "wo")) mode = O_WRONLY;
    else if (!strcmp(trozos[2], "rw")) mode = O_RDWR;
    else if (!strcmp(trozos[2], "ap")) mode = O_APPEND; 
    else if (!strcmp(trozos[2], "tr")) mode = O_TRUNC;
    else {
        printf("Error: modo inválido '%s'\n", trozos[2]);
        return;
    }

    df = open(trozos[1], mode, 0666);
    if (df == -1) {
        perror("Error al abrir el archivo");
        return;
    }

    AgregarArchivo(&listaArchivos, trozos[1], df, mode);
    printf("Archivo abierto: %s, Descriptor: %d\n", trozos[1], df);
}

void cmd_close(char *trozos[]){
  if (trozos[1] == NULL){
        // Si no hay argumento, muestra archivos abiertos
        ListarFicherosAbiertos(&listaArchivos);
        return;
    }
    int df = atoi(trozos[1]);
    if (df < 0){
        printf("Descriptor inválido.\n");
        return;
    }
    // Comprobar si el descriptor existe en la lista
    NodoArchivo *aux = listaArchivos.primero;
    bool encontrado = false;
    while (aux != NULL){
        if (aux->descriptor == df){
            encontrado = true;
            break;
        }
       aux = aux->siguiente;
    }
    if (!encontrado){
        printf("No existe archivo con descriptor %d\n", df);
        return;
    }
    // Cerrar descriptor y eliminar de la lista
    if (close(df) == -1){
        printf("Error al cerrar el archivo\n");
        return;
    }
    EliminarDeFicherosAbiertos(&listaArchivos, df);
    printf("Archivo con descriptor %d cerrado correctamente\n", df);
}

void cmd_dup(char *trozos[]) {
  int df, copia;
  char *nombre;
  char aux[MAX_TOKENS];

  if (trozos[1] == NULL || (df=atoi(trozos[1]))<0){
    ListarFicherosAbiertos(&listaArchivos);
    return;
  }

  copia = dup(df);
  if (copia == -1){
    printf("Error al duplicar el archivo\n");
    return;
  }
  nombre = NombreFicheroDescriptor(df, &listaArchivos);
  if (nombre == NULL){
    printf("No se encontró el archivo con descriptor: %d\n", df);
    close(copia);
    return;
  }
  sprintf(aux, "dup %d (%s)", df, nombre);
  AgregarArchivo(&listaArchivos, aux, copia, 0);
  printf("Archivo con descriptor: %d  duplicado correctamente\n", copia);
}

void cmd_listopen(char *trozos[]){
  if(listaArchivos.primero==NULL) {
  printf("No hay archivos abiertos.\n");
  return;
}
  ListarFicherosAbiertos(&listaArchivos);
}

void cmd_infosys(char *trozos[]){
   if (trozos[1] != NULL) {
        printf("Opción no válida para 'infosys'.\n");
        return;
    }
    
    
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
    } else if(strcmp(trozos[1], "hour") == 0){
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

void cmd_exit(char* trozos[]){
  exit(0);
  }
