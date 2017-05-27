#include "arista.h"

//Inicializa la arista
Arista* arista_init(int ID, struct nodo* nodo1, struct nodo* nodo2, int capacidad, int k){
  Arista* arista = malloc(sizeof(Arista));
  arista -> ID = ID;
  arista -> nodo1 = nodo1;
  nodo_aumentar_conexiones(nodo1);
  arista -> nodo2 = nodo2;
  nodo_aumentar_conexiones(nodo2);
  arista -> capacidad = capacidad;
  arista -> flujo = 0;
  arista -> conectado = false;
  arista -> k = k;
  return arista;
}

//Destruye el la arista
void arista_destroy(Arista* arista){
  free(arista);
}
