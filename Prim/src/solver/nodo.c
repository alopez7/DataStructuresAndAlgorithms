#include "nodo.h"
#include "utiles.h"

//Inicializa el nodo
Nodo* nodo_init(int ID, int k){
  Nodo* nodo = malloc(sizeof(Nodo));
  nodo -> ID = ID;
  nodo -> conectado = false;
  nodo -> flujo = 0;
  nodo -> k = k;
  nodo -> n_conexiones = 0;
  nodo -> a_agregadas = 0;
  return nodo;
}

//Suma 1 a la cantidad de conexiones
void nodo_aumentar_conexiones(Nodo* nodo){
  nodo -> n_conexiones++;
}

//dice si esta conectado o no
bool nodo_conectado(Nodo* nodo){
  return nodo -> conectado;
}

//retorna el flujo del nodo
int nodo_flujo(Nodo* nodo){
  return nodo -> flujo;
}


//Da el nodo contiguo a travez de la arista dada
Nodo* nodo_otro_nodo(Nodo* nodo, Arista* arista){
  if (arista -> nodo1 == nodo){
    return arista -> nodo2;
  }
  return arista -> nodo1;
}

//Destruye el nodo
void nodo_destroy(Nodo* nodo){
  free(nodo -> aristas);
  free(nodo);
}
