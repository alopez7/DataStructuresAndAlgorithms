#include "grafo.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

Grafo* parsear(char* filename){
  FILE* f = fopen(filename, "r");
  char buf[256];
  int n_vertices;
  int n_nodo_f;
  int k;
  int n_aristas;
  int inutil;
  inutil = fscanf(f,"%s", buf);
  inutil = fscanf(f,"%i", &n_vertices);
  inutil = fscanf(f,"%s", buf);
  inutil = fscanf(f,"%i", &n_nodo_f);
  inutil = fscanf(f,"%s", buf);
  inutil = fscanf(f,"%i", &k);
  inutil = fscanf(f,"%s", buf);
  inutil = fscanf(f,"%i", &n_aristas);
  Nodo** array_nodos = malloc(sizeof(Nodo*) * n_vertices);
  Nodo* aux;
  for (int i = 0; i < n_vertices; i++){
    aux = nodo_init(i, k);
    array_nodos[i] = aux;
  }
  int id;
  int n_nodo1;
  int n_nodo2;
  int cap;
  Arista** array_aristas = malloc(sizeof(Arista*) * n_aristas);
  Arista* aaux;
  Nodo* nodo1;
  Nodo* nodo2;
  for (int i = 0; i < n_aristas; i++){
    inutil = fscanf(f,"%i", &id);
    inutil = fscanf(f,"%i", &n_nodo1);
    inutil = fscanf(f,"%i", &n_nodo2);
    inutil = fscanf(f,"%i", &cap);
    nodo1 = array_nodos[n_nodo1];
    nodo2 = array_nodos[n_nodo2];
    aaux = arista_init(id, nodo1, nodo2, cap, k);
    array_aristas[i] = aaux;
  }
  Heap* heap = heap_init(n_aristas, k);
  Grafo* g = grafo_init(array_nodos, array_aristas, k, n_nodo_f, heap, n_aristas, n_vertices);
  fclose(f);
  inutil = inutil + 1;
  return g;
}

//Ejecución del programa
int main(int argc, char *argv[])
{
  if (argc != 2){
    printf("Acuerdate que debes dar el path al archivo y nada mas\n");
    return 1;
  }
  //Obtengo el grafo del archivo
  Grafo* grafo = parsear(argv[1]);
  for (int i = 0; i < grafo -> n_nodos - 1; i++){
    grafo_iterar(grafo);
  }
  for (int i = 0; i < grafo -> n_nodos - 1; i++){
    printf("%i\n", grafo -> respuesta[i] -> ID);
  }
  grafo_destroy(grafo);
  return 0;
}
