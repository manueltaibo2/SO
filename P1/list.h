/*
Antón Vázquez López anton.vazquez.lopez@udc.es
Manuel Taibo González manuel.taibo2@udc.es
*/

#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Nodo {
    char *comando;
    struct Nodo *siguiente;
} Nodo;

typedef struct Lista{
    Nodo *primero;
    int tamaño;
} Lista;

typedef struct NodoArchivo {
    int descriptor;
    char *nombre;
    int modo;
    struct NodoArchivo *siguiente;
} NodoArchivo;

typedef struct ListaArchivos{
    NodoArchivo *primero;
} ListaArchivos;

int TrocearCadena(char *cadena, char *trozos[]);

void InicializarLista (Lista *lista);
bool CrearNodo (Nodo **nuevoNodo, char *comando);
void AgregarComando (Lista *lista, char *comando);
void ImprimirComandos (Lista lista);
void ImprimirComandoN(Lista lista, int Ncomando);
void ImprimirComandoMN(Lista lista, int Ncomando);
void LiberarLista (Lista *Lista);
void EliminarNodo (Lista *Lista, int position);


void InicializarListaArchivos (ListaArchivos *lista);
bool CrearNodoArchivo(NodoArchivo **nuevoNodo, const char *nombre, int descriptor, int modo);
void AgregarArchivo(ListaArchivos *lista, const char *nombre, int descriptor, int modo);
void ListarFicherosAbiertos(ListaArchivos *lista);
void EliminarDeFicherosAbiertos (ListaArchivos *lista, int descriptor);
char* NombreFicheroDescriptor(int descriptor, ListaArchivos *lista);
void LiberarListaArchivos(ListaArchivos *lista);

#endif
