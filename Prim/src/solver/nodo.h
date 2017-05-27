#ifndef T2_LIB_NODO
#define T2_LIB_NODO

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "arista.h"

struct nodo
{
  //Un id
  int ID;
  //Arreglo de aristas
  Arista** aristas;
  //El flujo que pasa por el nodo
  int flujo;
  //Determina si se conecto a la red
  bool conectado;
  //k
  int k;
  //cantidad de conexiones
  int n_conexiones;
  //cantidad de aristas agregadas
  int a_agregadas;
};

typedef struct nodo Nodo;

//Inicializa el nodo
Nodo*    nodo_init(int ID, int k);

//Suma 1 a la cantidad de conexiones
void     nodo_aumentar_conexiones(Nodo* nodo);

//dice si esta conectado o no
bool     nodo_conectado(Nodo* nodo);

//retorna el flujo del nodo
int      nodo_flujo(Nodo* nodo);

//Da el nodo contiguo a travez de la arista dada
Nodo*    nodo_otro_nodo(Nodo* nodo, Arista* arista);

//Destruye el nodo
void     nodo_destroy(Nodo* nodo);

#endif
