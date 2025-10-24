
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pwd.h>
#include <grp.h>
#include <sys/utsname.h>
#include <sys/stat.h>  // Para mkdir
#include <sys/types.h>
#include <dirent.h>   // Para opendir, readdir, closedir


#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include "list.h"
#include "comandos.h"
#include "aux.h"

#define BUFFER_SIZE 1024
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
    int i = 2;

    if (trozos[1] == NULL) {
        ListarFicherosAbiertos(&listaArchivos);
        return;
    }

    if (trozos[2] == NULL) {
        printf("Error: falta el modo de apertura (cr, ex, ro, wo, rw, ap, tr)\n");
        return;
    }

    // Procesar TODOS los modos proporcionados
    while (trozos[i] != NULL) {
        if (!strcmp(trozos[i], "cr")) mode |= O_CREAT;  
        else if (!strcmp(trozos[i], "ex")) mode |= O_EXCL; 
        else if (!strcmp(trozos[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(trozos[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(trozos[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(trozos[i], "ap")) mode |= O_APPEND; 
        else if (!strcmp(trozos[i], "tr")) mode |= O_TRUNC;
        else {
            printf("Error: modo inválido '%s'\n", trozos[i]);
            return;
        }
        i++;
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
        puts("  - hour: Muestra la hora en formato hh:mm:ss.");
        puts("  - historic: Muestra el historial de comandos.");
        puts("  - open: Abre un archivo.");
        puts("  - close: Cierra un archivo.");
        puts("  - dup: Duplica un archivo.");
        puts("  - listopen: Muestra todos los archivos abiertos.");
        puts("  - infosys: Muestra información del sistema.");
        puts("  - help: Muestra lista y descripción de comandos disponibles.");
        puts("  - exit: Sale del shell.");
        puts("  - create: Crea un archivo o directorio.");
        puts("  - erase: Borra un archivo o directorio.");
        puts("  - delrec: Borra un archivo o directorio de forma recursiva.");
        puts("  - lseek: Cambia el offset de un archivo abierto.");
        puts("  - dir: Lista archivos y directorios.");
        puts("  - setdirparams: Configura los parámetros del comando dir.");
        puts("  - getdirparams: Muestra el estado actual de los parámetros de dir.");
    } 
    else if(strcmp(trozos[1], "authors") == 0){
        printf("AUTHORS:  Muestra autores del programa.\n  - USO: authors [-l | -n]\n");
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
        printf("HISTORIC:  Muestra el historial de comandos.\n  - USO: historic [-N | -n | -clear | -count]\n");
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
    } else if(strcmp(trozos[1], "create") == 0){
        printf("CREATE: Crea un archivo o directorio.\n");
        printf("  - USO: create [-f] <nombre>\n  - create -f <nombre>: crea un archivo\n  - create <nombre>: crea un directorio\n");
    } else if(strcmp(trozos[1], "erase") == 0){
        printf("ERASE: Elimina archivos y/o directorios vacíos.\n");
        printf("  - USO: erase <nombre1> [nombre2] [nombre3] ...\n");
    } else if(strcmp(trozos[1], "delrec") == 0){
        printf("DELREC: Elimina archivos y/o directorios de forma recursiva.\n");
        printf("  - USO: delrec <nombre1> [nombre2] [nombre3] ...\n");
    } else if(strcmp(trozos[1], "lseek") == 0){
        printf("LSEEK: Cambia el offset de un archivo abierto.\n");
        printf("  - USO: lseek <descriptor> <offset> <whence>\n");
        printf("  - whence: SEEK_SET, SEEK_CUR, SEEK_END\n");
    } else if(strcmp(trozos[1], "dir") == 0){
        printf("DIR: Lista archivos y directorios.\n");
        printf("  - USO: dir [-d] [nombre1] [nombre2] ...\n");
        printf("  - Sin argumentos: lista el directorio actual.\n");
        printf("  - Con -d: lista el contenido del directorio.\n");
    } else if(strcmp(trozos[1], "setdirparams") == 0){
        printf("SETDIRPARAMS: Configura los parámetros del comando dir.\n");
        printf("  - USO: setdirparams [long|short] [link|nolink] [hid|nohid] [reca|recb|norec]\n");
        printf("  - Permite establecer varios parámetros en una misma llamada.\n");
    } else if(strcmp(trozos[1], "getdirparams") == 0){
        printf("GETDIRPARAMS: Muestra el estado actual de los parámetros de 'dir'.\n");
        printf("  - USO: getdirparams\n");
    } else {
        printf("Comando no encontrado: %s\n", trozos[1]);
    }
}

void cmd_exit(char* trozos[]){
  exit(0);
  }


// COMANDOS P1

void cmd_create (char* trozos[]){
    if (trozos[1]==NULL){
        printf("Error: falta el nombre del archivo a crear.\n");
        printf("Uso: create [-f] <nombre_archivo>\n");
        return;
    }

    // Caso: create -f nombre (crear archivo)
    if (strcmp(trozos[1], "-f")==0){
        if (trozos[2]==NULL){
            printf("Error: falta el nombre del archivo a crear.\n");
            printf("Uso: create [-f] <nombre_archivo>\n");
            return;
        }
        
        int fd = open(trozos[2], O_CREAT | O_EXCL, 0664);
        if (fd == -1){
            perror("Error al crear el archivo");
            return;
        }
        close(fd);
        printf("Archivo '%s' creado correctamente.\n", trozos[2]);
    } 
    // Caso: create nombre (crear directorio)
    else {
        if (mkdir(trozos[1], 0755) == -1){
            perror("Error al crear el directorio");
            return;
        }
        printf("Directorio '%s' creado correctamente.\n", trozos[1]);
    }
}

void cmd_erase (char* trozos[]){
    if (trozos[1]==NULL){
        printf("Error: falta el nombre del archivo o directorio a borrar.\n");
        printf("Uso: erase <nombres>\n");
        return;
    }
    for (int i=1; trozos[i]!=NULL; i++){
        struct stat info;
        
        if (stat(trozos[i], &info) == -1){
            printf("'%s': ", trozos[i]);
            perror("Error");
            continue;
        }
        
        if(S_ISDIR(info.st_mode)){
            // Es un directorio
            if (rmdir(trozos[i]) == -1){
                printf("'%s': ", trozos[i]);
                perror("Error al eliminar directorio");
            } else {
                printf("Directorio '%s' borrado correctamente.\n", trozos[i]);
            }
        } else {
            // Es un archivo
            if (unlink(trozos[i]) == -1){
                printf("'%s': ", trozos[i]);
                perror("Error al eliminar archivo");
            } else {
                printf("Archivo '%s' borrado correctamente.\n", trozos[i]);
            }
        }
    }
}

void aux_delrec(char* path){
    struct stat info;

    if(stat(path, &info) == -1){
        printf("'%s': ", path);
        perror("Error");
        return;
    }

    if(!S_ISDIR(info.st_mode)){
        // Es un archivo
        if(unlink(path) == -1){
            printf("'%s': ", path);
            perror("Error al eliminar archivo");
        } else {
            printf("Archivo '%s' borrado correctamente.\n", path);
        }
        return;
    }

    // Es un directorio
    DIR *dir = opendir(path);
    if(dir == NULL){
        printf("'%s': ", path);
        perror("Error al abrir directorio");
        return;
    }

    struct dirent *entry;
    char fullpath[BUFFER_SIZE];

    //recorrer el directorio
    while((entry = readdir(dir)) != NULL){
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0){
            continue; // Saltar . y ..
        }

        //construir la ruta completa
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        aux_delrec(fullpath); // llamada recursiva
    }

    closedir(dir);

    // Intentar eliminar el directorio ahora vacío
    if(rmdir(path) == -1){
        printf("'%s': ", path);
        perror("Error al eliminar directorio");
    } else {
        printf("Directorio '%s' borrado correctamente.\n", path);
    }

}    

void cmd_delrec(char* trozos[]){
    if (trozos[1]==NULL){
        printf("Error: falta el nombre del archivo o directorio a eliminar.\n");
        printf("Uso: delrec delrec <nombre1> [nombre2] [nombre3] ...\n");
        return;
    }
    
    for (int i=1; trozos[i]!=NULL; i++){
        aux_delrec(trozos[i]);
    }
}

void cmd_lseek(char* trozos[]){
    if (trozos[1]==NULL || trozos[2]==NULL || trozos[3]==NULL){
        printf("Error: falta argumentos.\n");
        printf("Uso: lseek <descriptor> <offset> <whence>\n");
        return;
    }

    // Convertir argumentos
    int df = atoi(trozos[1]);
    if (df < 0){
        printf("Descriptor inválido.\n");
        return; 
    }
    
    long offset = atol(trozos[2]);
    int whence;

    // Determinar el valor de 'whence'
    if (strcmp(trozos[3], "SEEK_SET") == 0) whence = SEEK_SET;
    else if (strcmp(trozos[3], "SEEK_CUR") == 0) whence = SEEK_CUR;
    else if (strcmp(trozos[3], "SEEK_END") == 0) whence = SEEK_END;
    else {
        printf("Valor inválido para 'whence'. Use SEEK_SET, SEEK_CUR o SEEK_END.\n");
        return;
    }

    NodoArchivo *aux = listaArchivos.primero;
    bool encontrado = false;

    // Comprobar si el descriptor existe en la lista
    while (aux != NULL){
        if (aux->descriptor == df){
            encontrado = true;
            break;
        }
       aux = aux->siguiente;
    }

    // Descriptor no está en la lista
    if (!encontrado){
        printf("No existe archivo con descriptor %d\n", df);
        return;
    }

    // Verificar que no sea stdin, stdout o stderr**
    if (df == 0 || df == 1 || df == 2){
        printf("No se puede hacer lseek sobre descriptores estándar (stdin/stdout/stderr)\n");
        return;
    }

    // Realizar lseek
    off_t nueva_pos = lseek(df, offset, whence);
    if (nueva_pos == (off_t)-1){
        perror("Error en lseek");
        return;
    }

    printf("Offset actualizado a la posición: %ld\n", nueva_pos);
}

void cmd_writestr(char* trozos[]){
    if (trozos[1] == NULL || trozos[2] == NULL){
        printf("Error: faltan argumentos.\n");
        printf("Uso: writestr <descriptor> <cadena>\n");
        return;
    }
    
    int df = atoi(trozos[1]);
    if (df < 0){
        printf("Descriptor inválido.\n");
        return;
    }

    NodoArchivo *aux = listaArchivos.primero;
    bool encontrado = false;

    // Compruebo  si el descriptor existe en la lista
    while (aux != NULL){
        if (aux->descriptor == df){
            encontrado = true;
            break;
        }
        aux = aux->siguiente;
    }

    // El descriptor no está en la lista
    if (!encontrado){
        printf("No existe archivo con descriptor %d\n", df);
        return;
    }

    //Extraer permisos del archivo
    int accmode = aux->modo & O_ACCMODE;  // O_ACCMODE = 0003 (máscara para extraer ro/wo/rw)
    
    if (accmode == O_RDONLY) {
        printf("El archivo con descriptor %d no tiene permisos de escritura.\n", df);
        return;
    }

    // Reconstruir la cadena completa desde trozos[2]
    char buffer[BUFFER_SIZE] = "";
    int i = 2;
    while (trozos[i] != NULL) {
        strcat(buffer, trozos[i]);
        if (trozos[i + 1] != NULL) {
            strcat(buffer, " ");  // Espacio entre palabras
        }
        i++;
    }

    // Escribir cadena en el archivo
    ssize_t bytes_escritos = write(df, buffer, strlen(buffer));
    if (bytes_escritos == -1){
        perror("Error al escribir en el archivo");
        return;
    } else {
        printf("Se han escrito %zd bytes en el archivo con descriptor %d.\n", bytes_escritos, df);
    }
}

void cmd_setdirparams(char *trozos[]) {
    if (trozos[1] == NULL) {
        printf("Uso: setdirparams <long|short> <link|nolink> <hid|nohid> <reca|recb|norec>\n");
        return;
    }

    for (int i = 1; trozos[i] != NULL; i++) {
        if (strcmp(trozos[i], "long") == 0)
            dirParams.longFormat = 1;
        else if (strcmp(trozos[i], "short") == 0)
            dirParams.longFormat = 0;

        else if (strcmp(trozos[i], "link") == 0)
            dirParams.showLinks = 1;
        else if (strcmp(trozos[i], "nolink") == 0)
            dirParams.showLinks = 0;

        else if (strcmp(trozos[i], "hid") == 0)
            dirParams.showHidden = 1;
        else if (strcmp(trozos[i], "nohid") == 0)
            dirParams.showHidden = 0;

        else if (strcmp(trozos[i], "reca") == 0)
            dirParams.recursion = 1;
        else if (strcmp(trozos[i], "recb") == 0)
            dirParams.recursion = 2;
        else if (strcmp(trozos[i], "norec") == 0)
            dirParams.recursion = 0;

        else {
            printf("Parámetro desconocido: %s\n", trozos[i]);
        }
    }
    
}

void cmd_getdirparams(char *trozos[]) {
    if (trozos[1] != NULL) {
        printf("Opción no válida para 'getdirparams'.\n");
        return;
    }

    printf("Listado: ");

    // Formato largo/corto
    if (dirParams.longFormat)
        printf("largo ");
    else
        printf("corto ");

    // Links
    if (dirParams.showLinks)
        printf("con links ");
    else
        printf("sin links ");

    // Recursividad
    switch (dirParams.recursion) {
        case 1:
            printf("recursivo (despues) ");
            break;
        case 2:
            printf("recursivo (antes) ");
            break;
        default:
            printf("no recursivo ");
            break;
    }

    // Ficheros ocultos
    if (dirParams.showHidden)
        printf("con ficheros ocultos\n");
    else
        printf("\n");
}
void listarArchivo(char *nombre) {
    struct stat s;
    if (lstat(nombre, &s) == -1) {
        perror("Error al obtener información del archivo");
        return;
    }

    // Oculto archivos ocultos si no se piden que se muestren
    char *base = strrchr(nombre, '/');
    base = (base) ? base + 1 : nombre;
    if (!dirParams.showHidden && base[0] == '.')
        return;

    if (dirParams.longFormat) {
        // Información de usuario y grupo
        struct passwd *pw = getpwuid(s.st_uid);
        struct group  *gr = getgrgid(s.st_gid);

        // Fecha
        struct tm *t = localtime(&s.st_mtime);
        char fecha[20];
        strftime(fecha, sizeof(fecha), "%Y/%m/%d-%H:%M", t);

        // Permisos
        char *permisos = ConvierteModo2(s.st_mode);

        printf("%s   %lu (%lu)    %s    %s %s %5ld %s",
               fecha,
               s.st_nlink,
               s.st_ino,
               pw ? pw->pw_name : "???",
               gr ? gr->gr_name : "???",
               permisos,
               s.st_size,
               base);

        // Enlaces simbólicos
        if (dirParams.showLinks && S_ISLNK(s.st_mode)) {
            char destino[PATH_MAX];
            ssize_t len = readlink(nombre, destino, sizeof(destino) - 1);
            if (len != -1) {
                destino[len] = '\0';
                printf(" -> %s", destino);
            }
        }

        printf("\n");
    } else {
        // Formato corto
        printf("%ld  %s \n", s.st_size, base);
    }
}

void listarDirectorio(const char *nombre) {
   DIR *dir;
    struct dirent *entry;

    dir = opendir(nombre);
    if (!dir) {
        perror("Error al abrir directorio");
        return;
    }

    printf("************%s\n", nombre);

    //  listamos los archivos del directorio actual
    while ((entry = readdir(dir)) != NULL) {
        // Saltamos los ocultos si no deben mostrarse
        if (!dirParams.showHidden && entry->d_name[0] == '.')
            continue;

        char ruta[PATH_MAX];
        snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entry->d_name);

        // Mostrar archivos y directorios (pero no recursivamente )
        listarArchivo(ruta);
    }

    closedir(dir);

    // recorremos recursivamente los subdirectorios
    dir = opendir(nombre);
    if (!dir) return;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        if (!dirParams.showHidden && entry->d_name[0] == '.')
            continue;

        char ruta[PATH_MAX];
        snprintf(ruta, sizeof(ruta), "%s/%s", nombre, entry->d_name);

        if (EsDirectorio(ruta))
            listarDirectorio(ruta);
    }

    closedir(dir);
}

void cmd_dir(char *trozos[]) {
     int listarContenido = 0;
    int startIndex = 1;

    // Si no hay argumentos, lista el directorio actual
    if (trozos[1] == NULL) {
        listarArchivo(".");
        return;
    }

    // Si el primer argumento es "-d", entonces lista el contenido de los directorios
    if (strcmp(trozos[1], "-d") == 0) {
        listarContenido = 1;
        startIndex = 2;
    }

    // Recorremos todos los argumentos
    for (int i = startIndex; trozos[i] != NULL; i++) {
        if (EsDirectorio(trozos[i])) {
            if (listarContenido)
                listarDirectorio(trozos[i]);   // Si hay -d listamos su contenido
            else
                listarArchivo(trozos[i]);      // Sin -d mostramos  solo la info del directorio
        } else {
            listarArchivo(trozos[i]);          // Es un archivo normal
        }
    }
}
