/*
Antón Vázquez López anton.vazquez.lopez@udc.es
Manuel Taibo González manuel.taibo2@udc.es
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int TrocearCadena(char * cadena, char * trozos[])
{ int i=1;

  if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
    return 0;
  while ((trozos[i]=strtok(NULL," \n\t"))!=NULL)
    i++;
  return i;
}

void InicializarLista (Lista *lista){
	lista->primero = NULL;  //Lista vacia
	lista->tamaño = 0;
}

void InicializarListaArchivos(ListaArchivos *lista) {
    lista->primero = NULL;
}

bool CrearNodo (Nodo **nuevoNodo, char *comando) {
    *nuevoNodo = (Nodo*) malloc(sizeof(Nodo));  //Reserva memoria.
    if (*nuevoNodo==NULL) return false;

    (*nuevoNodo)->comando = (char*) malloc(strlen(comando)+1); //Usamos +1 por \0.
    if ((*nuevoNodo)->comando==NULL) {
        free(*nuevoNodo);
        return false;
    }

    strcpy((*nuevoNodo)->comando, comando);
    (*nuevoNodo)->siguiente = NULL;
    return true;
}

bool CrearNodoArchivo (NodoArchivo **nuevoNodo, const char *nombre, int descriptor, int modo) {
    *nuevoNodo = (NodoArchivo*) malloc(sizeof(NodoArchivo));
    if (*nuevoNodo==NULL) return false;

    (*nuevoNodo)->nombre = (char*) malloc(strlen(nombre)+1);
    if ((*nuevoNodo)->nombre==NULL) {
        free(*nuevoNodo);
        return false;
    }

    strcpy((*nuevoNodo)->nombre, nombre);
    (*nuevoNodo)->descriptor = descriptor;
    (*nuevoNodo)->modo = modo;
    (*nuevoNodo)->siguiente = NULL;
    return true;
}

void AgregarComando (Lista *lista, char *comando) {
    Nodo *nuevoNodo;

    if (!CrearNodo(&nuevoNodo, comando)) {
        printf("Error al agregar comando\n");
        return;
    }

  if (lista->primero == NULL) {
    lista->primero = nuevoNodo;
    lista->tamaño++;
  } else {
    Nodo *aux = lista->primero;
    while (aux->siguiente!=NULL) aux = aux->siguiente;
    aux->siguiente = nuevoNodo;
    lista->tamaño++;
  }
}

void AgregarArchivo(ListaArchivos *lista, const char *nombre, int descriptor, int modo) {
    NodoArchivo *nuevoNodo;

    if (!CrearNodoArchivo(&nuevoNodo, nombre, descriptor, modo)) {
        printf("Error al agregar comando\n");
        return;
    }

    if (lista->primero == NULL) lista->primero = nuevoNodo;
    else {
        NodoArchivo *aux = lista->primero;
        while (aux->siguiente!=NULL) aux = aux->siguiente;
        aux->siguiente = nuevoNodo;
    }

    //printf("Archivo agregado: Descriptor: %d, Nombre: %s, Modo: %d\n", descriptor, nombre, modo);
}

void ImprimirComandos(Lista lista) {
  Nodo *aux = lista.primero;
  char *trozos[50];
  char *copia;
  int numTrozos = 0;

  if(aux==NULL){
    printf("Lista vacia\n");
    return;
  }

  while (aux!=NULL) {
    copia = (char*) malloc(strlen(aux->comando)+1);
    strcpy(copia, aux->comando);

    numTrozos = TrocearCadena(copia, trozos);
    for (int i=0; i<numTrozos; i++) {
      printf("%s ", trozos[i]);
    }
    printf("\n--------\n");
    free(copia);
    aux = aux->siguiente;
  }
}

void ImprimirComandoN (Lista lista, int nComando){
  if(nComando>=lista.tamaño){
    printf("No existe el comando con número %d\n", nComando);
    return;
  }
  Nodo *aux = lista.primero;
  char *trozos[10];
  char *copia;
  int numTrozos = 0;

  if(aux==NULL){
    printf("Lista vacia\n");
    return;
  }

  for (int i=0; i<nComando; i++) aux = aux->siguiente;
  copia = (char*) malloc(strlen(aux->comando)+1);
  strcpy(copia, aux->comando);
  numTrozos = TrocearCadena(copia, trozos);
  printf("\t");
  for (int i=0; i<numTrozos; i++) {
    printf("%s ", trozos[i]);
  }
  puts("");
  free(copia);
  aux = aux->siguiente;
}

void ImprimirComandoMN(Lista lista, int Ncomando) {
  if (Ncomando <= 0 || Ncomando > lista.tamaño) {
      printf("No se han escrito %d comandos \n", Ncomando);
      return;
  }

  Nodo* aux = lista.primero;
  char *trozos[10];
  char *copia;
  int num_trozos = 0;
  int dest = lista.tamaño - Ncomando; //Calcula desde donde hay que empezar
  //Recorre la lista hasta ese comando
  for (int cnt = 0; cnt < dest; cnt++) {
      aux = aux->siguiente;
  } //Imprime desde N hasta el final de la lista.
  for (int i = dest; i < lista.tamaño; i++) {
      if (aux != NULL) {
          copia = (char*) malloc(strlen(aux->comando) + 1);
          strcpy(copia, aux->comando);

          num_trozos = TrocearCadena(copia, trozos);
          printf("\t");
          for(int j = 0; j < num_trozos; j++){  //Imprime los trozos
              printf("%s ", trozos[j]);
          }
		  puts("");
          aux = aux->siguiente;
          free(copia);
        }
    }
}

void ListarFicherosAbiertos(ListaArchivos *lista){
  if (lista == NULL) {
  	printf("Lista no inicializada\n");
    return;
  }

  NodoArchivo* aux = lista->primero;

  if (aux == NULL) {
	printf("No hay archivos abiertos\n");
    return;
  }

  printf("Archivos abiertos:\n");
  while (aux != NULL) {
    printf("Descriptor: %d, Nombre: %s, Modo: %d\n",
    		aux->descriptor, aux->nombre, aux->modo);
    aux = aux->siguiente;
  }
}

void EliminarDeFicherosAbiertos (ListaArchivos *lista, int descriptor){
  NodoArchivo *aux = lista->primero;
  NodoArchivo *ant = NULL;

  while (aux != NULL && aux->descriptor != descriptor){
    ant = aux;
    aux = aux->siguiente;
  }

  if (aux==NULL){
    printf("Archivo con descriptor %d no existe\n", descriptor);
    return;
  }

  if (ant==NULL) lista->primero = aux->siguiente;
  else ant->siguiente = aux->siguiente;

  free(aux->nombre);
  free(aux);
  //printf("El archivo con descriptor %d ha sido eliminado\n", descriptor);
}

char* NombreFicheroDescriptor(int descriptor, ListaArchivos *lista){
  NodoArchivo *aux = lista->primero;
  while (aux != NULL){
    if (aux->descriptor == descriptor) return aux->nombre;
    aux = aux->siguiente;
  }
  return NULL;
}

void LiberarLista(Lista *lista){
  Nodo *aux = lista->primero;
  Nodo *sig;

  while (aux != NULL){
    sig = aux->siguiente;
    free(aux->comando);
    free(aux);
    aux = sig;
  }

  InicializarLista(lista);
}

void LiberarListaArchivos (ListaArchivos *lista){
  NodoArchivo *aux = lista->primero;
  NodoArchivo *sig;

  while (aux != NULL){
    sig = aux->siguiente;
    free(aux->nombre);
    free(aux);
    aux = sig;
  }
  InicializarListaArchivos(lista);
}

void EliminarNodo(Lista *lista, int pos){
  if (lista == NULL || pos >= lista->tamaño) return;

  Nodo *aux = lista->primero;
  Nodo *ant = NULL;

  if (pos == 0) {
    lista->primero = aux->siguiente;
    free(aux->comando);
    free(aux);
    lista->tamaño--;
    return;
  }

  for (int i = 0; i < pos; i++) {
    ant = aux;
    aux = aux->siguiente;
  }

  ant->siguiente = aux->siguiente;
  free(aux->comando);
  free(aux);
  lista->tamaño--;
}
