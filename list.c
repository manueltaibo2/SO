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

void AñadirFicherosAbiertos (){
  
}

void ListarFicherosAbiertos(ListaArchivos *lista){
  NodoArchivo *aux= lista->primero;
  while (aux != NULL){ //Recorre toda la lista imprimiendo los datos de los archivos
		printf("Descriptor: %d, Nombre: %s, Modo: %d\n", aux->descriptor,aux->nombre,aux->modo);
		aux = aux->siguiente;
	}
}

void EliminarDeFicherosAbiertos (){
  
}
