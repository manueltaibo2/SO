#ifndef aux_h
#define aux_h



#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


struct DirParams {
    int longFormat;  // 1 = long, 0 = short
    int showLinks;   // 1 = link, 0 = nolink
    int showHidden;  // 1 = hid, 0 = nohid
    int recursion;   // 0 = norec, 1 = reca, 2 = recb
};


extern struct DirParams dirParams;

// Funciones de ayuda para permisos
char LetraTF(mode_t m);
char* ConvierteModo2(mode_t m);
int EsDirectorio(char* dir);

#endif
