#include "heap.h"

//Inicializa el heap
Heap* heap_init(int n_aristas_maximo, int k){
  Heap* heap = malloc(sizeof(Heap));
  heap -> k = k;
  //inicializo el arreglo con tamano maximo que puede llegar a tener
  heap -> arreglo = malloc(sizeof(Arista*) * n_aristas_maximo);
  //La cantidad de items que tiene
  heap -> n_items = 0;
  return heap;
}

//Imprime el heap
void heap_imprimir(Heap* heap){
  printf("K: %i\n", heap -> k);
  int contador = 1;
  int limite = 1;
  for (int i = 1; i <= heap -> n_items; i++){
    Arista* a = heap_elemento(heap, i);
    printf("%i->f:%i,c:%i.", i, a -> flujo, a -> capacidad);
    if (i == limite){
      printf("\n");
      contador++;
      limite = dos_a_la(contador) - 1;
    }
  }
  printf("\nn_elems: %i\n", heap -> n_items);
}

//Elimina el primer término y lo retorna
Arista* heap_pop(Heap* heap){
  Arista* maximo = heap_elemento(heap, 1);
  heap_asignar(heap, heap_elemento(heap, heap -> n_items), 1);
  heap -> n_items = heap -> n_items - 1;
  heap_heapear_pop(heap, 1);
  return maximo;
}

//Agrega una arista
void heap_insertar(Heap* heap, Arista* arista){
  heap -> n_items++;
  heap_asignar(heap, arista, heap -> n_items);
  heap_heapear_insert(heap, heap -> n_items);
}

//Cambia de posicion 2 aristas
void heap_intercambiar(Heap* heap, int pos1, int pos2){
  Arista* aux = heap_elemento(heap, pos1);
  heap_asignar(heap, heap_elemento(heap, pos2), pos1);
  heap_asignar(heap, aux, pos2);
}

//Lleva al heap a sus propiedades recursivamente luego del pop
void heap_heapear_pop(Heap* heap, int malo){
  Arista* arista = heap_elemento(heap, malo);
  Arista* hijo1;
  Arista* hijo2;
  int n_hijos = 0;
  if (heap -> n_items >= malo * 2 + 1){
    hijo2 = heap_elemento(heap, malo * 2 + 1);
    hijo1 = heap_elemento(heap, malo * 2);
    n_hijos = 2;
  }
  else if (heap -> n_items == malo * 2){
    hijo1 = heap_elemento(heap, malo * 2);
    n_hijos = 1;
  }

  if (n_hijos == 2){
    if (heap_mejor_arista(hijo1, arista) && heap_mejor_arista(hijo1, hijo2)){
      heap_intercambiar(heap, malo, malo * 2);
      heap_heapear_pop(heap, malo * 2);
    }
    else if (heap_mejor_arista(hijo2, arista) && heap_mejor_arista(hijo2, hijo1)){
      heap_intercambiar(heap, malo, malo * 2 + 1);
      heap_heapear_pop(heap, malo * 2 + 1);
    }
  }
  else if (n_hijos == 1){
    if (heap_mejor_arista(hijo1, arista)){
      heap_intercambiar(heap, malo, malo * 2);
    }
  }
}

//Lleva al heap a sus propiedades recursivamente luego del insert
void heap_heapear_insert(Heap* heap, int malo){
  if (malo != 1){
    int pos_padre = malo / 2;
    Arista* padre = heap_elemento(heap, pos_padre);
    Arista* hijo = heap_elemento(heap, malo);
    if (heap_mejor_arista(hijo, padre)){
      heap_intercambiar(heap, pos_padre, malo);
      heap_heapear_insert(heap, pos_padre);
    }
  }
}

//Da el elemento de la posicion dada (posicion desde 1 hasta n)
Arista* heap_elemento(Heap* heap, int posicion){
  return heap -> arreglo[posicion - 1];
}

//Pone en la posicion n, la arista dada
void heap_asignar(Heap* heap, Arista* arista, int n){
  heap -> arreglo[n - 1] = arista;
}

//True si la major arista es la primera, false de lo contrario
bool heap_mejor_arista(Arista* a1, Arista* a2){
  if (a1 -> flujo < a2 -> flujo){
    return false;
  }
  else if (a2 -> flujo < a1 -> flujo){
    return true;
  }
  else {
    if (a1 -> capacidad <= a2 -> capacidad){
      return true;
    }
    return false;
  }
}

//Destruye el heap
void heap_destroy(Heap* heap){
  free(heap -> arreglo);
  free(heap);
}
