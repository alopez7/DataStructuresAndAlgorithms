#ifndef T2_LIB_GRAFO
#define T2_LIB_GRAFO

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "heap.h"
#include "nodo.h"


/** Estructura nodo*/
struct grafo
{
  //Arreglo con los nodos del grafo
  Nodo** nodos;
  //Arreglo con las aristas del grafo
  Arista** aristas;
  //K
  int k;
  //Heap con aristas
  Heap* heap;
  //Nodo f
  Nodo* nodo_f;
  //Numero de nodos
  int n_nodos;
  //Numero de aristas
  int n_aristas;
  //Aristas finales
  Arista** respuesta;
  //Cantidad de respuestas
  int n_respuesta;
};

//Funciones
typedef struct grafo Grafo;

//Inicializa el grafo
Grafo*   grafo_init(Nodo** nodos, Arista** aristas, int k, int n_nodo_f, Heap* heap, int n_aristas, int n_nodos);

//une los nodos a travez de las aristas
void     grafo_unir_nodos(Grafo* grafo);

//realiza las secuencias de iteracion
void     grafo_iterar(Grafo* grafo);

//Calcula SUM(W)
double   grafo_sumw(Grafo* grafo);

//Calcula SUM(H)
double   grafo_sumh(Grafo* grafo);

//Destruye el grafo
void     grafo_destroy(Grafo* grafo);

#endif
