#include "aux.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>



// Inicialización de la variable global
struct DirParams dirParams = {0, 0, 0, 0};

// Devuelve el tipo de fichero
char LetraTF(mode_t m)
{
    switch (m & S_IFMT) {
        case S_IFSOCK: return 's';
        case S_IFLNK:  return 'l';
        case S_IFREG:  return '-';
        case S_IFBLK:  return 'b';
        case S_IFDIR:  return 'd';
        case S_IFCHR:  return 'c';
        case S_IFIFO:  return 'p';
        default:       return '?';
    }
}

// Convierte los permisos a rwx------ formato
char* ConvierteModo2(mode_t m)
{
    static char permisos[12];
    strcpy(permisos, "----------");

    permisos[0] = LetraTF(m);
    if (m & S_IRUSR) permisos[1] = 'r';
    if (m & S_IWUSR) permisos[2] = 'w';
    if (m & S_IXUSR) permisos[3] = 'x';
    if (m & S_IRGRP) permisos[4] = 'r';
    if (m & S_IWGRP) permisos[5] = 'w';
    if (m & S_IXGRP) permisos[6] = 'x';
    if (m & S_IROTH) permisos[7] = 'r';
    if (m & S_IWOTH) permisos[8] = 'w';
    if (m & S_IXOTH) permisos[9] = 'x';
    if (m & S_ISUID) permisos[3] = 's';
    if (m & S_ISGID) permisos[6] = 's';
    if (m & S_ISVTX) permisos[9] = 't';

    return permisos;
}

// Devuelve 1 si es directorio, 0 si no
int EsDirectorio(char* dir)
{
    struct stat s;
    if (lstat(dir, &s) == -1) return 0;
    return S_ISDIR(s.st_mode);
}
