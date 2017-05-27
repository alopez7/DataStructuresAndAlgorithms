#ifndef T2_LIB_ARISTA
#define T2_LIB_ARISTA

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "utiles.h"

struct nodo;
bool nodo_conectado(struct nodo* nodo);
int nodo_flujo(struct nodo* nodo);
void nodo_aumentar_conexiones(struct nodo* nodo);

/** Estructura arista*/
struct arista
{
  //Un id
  int ID;
  //Nodo1
  struct nodo* nodo1;
  //Nodo2
  struct nodo* nodo2;
  //El flujo que pasa por la arista
  int flujo;
  //Capacidad maxima de la arista
  int capacidad;
  //dice si esta conectado o no la arista
  bool conectado;
  //k
  int k;
};

//Funciones
typedef struct arista Arista;

//Inicializa la arista
Arista*  arista_init(int ID, struct nodo* nodo1, struct nodo* nodo2, int capacidad, int k);

//Destruye el nodo
void     arista_destroy(Arista* arista);

#endif
