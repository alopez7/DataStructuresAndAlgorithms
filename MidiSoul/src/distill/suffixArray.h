#ifndef T2_SUFFIX_ARRAY
#define T2_SUFFIX_ARRAY

#include "../music/melody.h"

/** Estructura que maneja el arreglo de sufijos*/
struct suffixArray
{
  /** Contiene el arreglo de punteros a los elementos (arreglo de sufijos)*/
  Element** S;

  /** Es el largo de sufArray*/
  int length;

  /** Arreglo de indices de los sufijos*/
  int* array;
};

typedef struct suffixArray SuffixArray;

/** recibe una piesa y retorna un suffix array*/
SuffixArray* suffixArray_init(Melody * melody);

/** Ordena el suffix array usando quicksort*/
void  suffixArray_quickSort(SuffixArray* sufArray, int i, int j);

/** Particiona el suffix array elgiendo como pivote el primer elemento*/
int   partition(SuffixArray* sufArray, int i, int j);

/** Cambia en el sufArray el indice en la posicion i con el de la posicion j*/
void  swap(SuffixArray* sufArray, int i, int j);

/** Compara dos sufijos del sufArray en las posiciones i y j retorna -1,0,1*/
int   cmp(SuffixArray* sufArray, int i, int j);

/** Crea un array de largo n-1 con la cantidad de elementos en comun de dos suficos ordenados consecutivos*/
int*  suffixArray_common_elements(SuffixArray* sufArray);

/** Calcula los elementos en comun entre los dos prefijos de i y j*/
int   prefix_common_elements(SuffixArray* sufArray, int i, int j);

/** Guarda en melody la frase que mas se repite, retorna el puntero a la cancion completa*/
Element*  find_frase(Melody* melody, SuffixArray* sufArray, int* common, int min_length);

/** Libera memoria del suffix array*/
void  suffixArray_destroy(SuffixArray* sufArray);

/** Compara dos Element*/
int element_cmp(Element e1, Element e2);

/** Imprime un Element*/
void element_print(Element el);

/** Transforma un array de notas a un array de diferencias de notas*/
void melody_diff(Melody* melody);

/** Transforma un array de diferencias de notas a un array de notas*/
void melody_undo_diff(Melody* melody);

#endif
