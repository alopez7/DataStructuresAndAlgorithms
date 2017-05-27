#ifndef T2_LIB_HEAP
#define T2_LIB_HEAP

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utiles.h"
#include "arista.h"

/** Estructura arista*/
struct heap
{
  //Un id
  int n_items;
  //Arreglo de aristas
  Arista** arreglo;
  //k
  int k;
};

//Funciones
typedef struct heap Heap;

//Inicializa el heap
Heap*     heap_init(int n_aristas_maximo, int k);

//Imprime el heap
void      heap_imprimir(Heap* heap);

//Elimina el primer término y lo retorna
Arista*   heap_pop(Heap* heap);

//Agrega una arista
void      heap_insertar(Heap* heap, Arista* arista);

//Cambia de posicion 2 aristas
void      heap_intercambiar(Heap* heap, int pos1, int pos2);

//Lleva al heap a sus propiedades recursivamente luego del pop
void      heap_heapear_pop(Heap* heap, int malo);

//Lleva al heap a sus propiedades recursivamente luego del insert
void      heap_heapear_insert(Heap* heap, int malo);

//Da el elemento de la posicion dada (posicion desde 1 hasta n)
Arista*   heap_elemento(Heap* heap, int posicion);

//Pone en la posicion n, la arista dada
void      heap_asignar(Heap* heap, Arista* arista, int n);

//True si la major arista es la primera, false de lo contrario
bool      heap_mejor_arista(Arista* a1, Arista* a2);

//Destruye el heap
void      heap_destroy(Heap* heap);

#endif
