#include "grafo.h"

//Inicializa el grafo
Grafo* grafo_init(Nodo** nodos, Arista** aristas, int k, int n_nodo_f, Heap* heap, int n_aristas, int n_nodos){
  Grafo* grafo = malloc(sizeof(Grafo));
  grafo -> aristas = aristas;
  grafo -> nodos = nodos;
  grafo -> heap = heap;
  grafo -> k = k;
  grafo -> nodo_f = nodos[n_nodo_f];
  grafo -> n_nodos = n_nodos;
  grafo -> n_aristas = n_aristas;
  grafo -> nodo_f -> flujo = k;
  grafo -> nodo_f -> conectado = true;
  grafo -> respuesta = malloc(sizeof(Arista*) * n_nodos - 1);
  grafo -> n_respuesta = 0;
  grafo_unir_nodos(grafo);
  Arista* aux;
  for (int i = 0; i < grafo -> nodo_f -> n_conexiones; i++){
    aux = grafo -> nodo_f -> aristas[i];
    aux -> flujo = minimo(grafo -> nodo_f -> flujo, aux -> capacidad);
    heap_insertar(heap, grafo -> nodo_f -> aristas[i]);
  }
  return grafo;
}

//une los nodos a travez de las aristas
void grafo_unir_nodos(Grafo* grafo){
  for (int i = 0; i < grafo -> n_nodos; i++){
    grafo -> nodos[i] -> aristas = malloc(sizeof(Arista*) * grafo -> nodos[i] -> n_conexiones);
  }
  Nodo* nodo1;
  Nodo* nodo2;
  for (int i = 0; i < grafo -> n_aristas; i++){
    nodo1 = grafo -> aristas[i] -> nodo1;
    nodo1 -> aristas[nodo1 -> a_agregadas] = grafo -> aristas[i];
    nodo1 -> a_agregadas++;
    nodo2 = grafo -> aristas[i] -> nodo2;
    nodo2 -> aristas[nodo2 -> a_agregadas] = grafo -> aristas[i];
    nodo2 -> a_agregadas++;
  }
}

//realiza las secuencias de iteracion
void grafo_iterar(Grafo* grafo){
  Arista* mejor = NULL;
  while (mejor == NULL){
    mejor = heap_pop(grafo -> heap);
    if (mejor -> conectado){
      mejor = NULL;
    }
  }
  mejor -> conectado = true;
  grafo -> respuesta[grafo -> n_respuesta] = mejor;
  grafo -> n_respuesta++;
  Nodo* nodo;
  if (mejor -> nodo1 -> conectado){
    nodo = mejor -> nodo2;
  }
  else {
    nodo = mejor -> nodo1;
  }
  nodo -> conectado = true;
  nodo -> flujo = mejor -> flujo;
  Arista* aux;
  for (int i = 0; i < nodo -> n_conexiones; i++){
    aux = nodo -> aristas[i];
    if (!nodo_otro_nodo(nodo, aux) -> conectado){
      aux -> flujo = minimo(nodo -> flujo, aux -> capacidad);
      heap_insertar(grafo -> heap, aux);
    }
    else {
      aux -> conectado = true;
    }
  }

}

//Calcula SUM(W)
double grafo_sumw(Grafo* grafo){
  double suma = 0;
  for (int i = 0; i < grafo -> n_respuesta; i++){
    suma = suma + grafo -> respuesta[i] -> capacidad;
  }
  return suma;
}

//Calcula SUM(H)
double grafo_sumh(Grafo* grafo){
  double suma = 0;
  for (int i = 0; i < grafo -> n_nodos; i++){
    suma = suma + grafo -> nodos[i] -> flujo;
  }
  return suma;
}

//Destruye el grafo
void grafo_destroy(Grafo* grafo){
  for (int i = 0; i < grafo -> n_aristas; i++){
    arista_destroy(grafo -> aristas[i]);
  }
  for (int i = 0; i < grafo -> n_nodos; i++){
    nodo_destroy(grafo -> nodos[i]);
  }
  heap_destroy(grafo -> heap);
  free(grafo -> aristas);
  free(grafo -> nodos);
  free(grafo -> respuesta);
  free(grafo);
}
