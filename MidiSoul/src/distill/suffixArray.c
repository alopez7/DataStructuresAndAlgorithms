#include <stdio.h>
#include "suffixArray.h"

/** recibe una piesa y retorna un suffix array*/
SuffixArray* suffixArray_init(Melody* melody)
{
  //Creo es espacio para el suffix array
  SuffixArray* sufArray = malloc(sizeof(SuffixArray));

  //Inicializo las variables
  sufArray -> length = melody -> length;

  //Creo el arraglo de sufijos y el arreglo de indices
  sufArray -> S = malloc(sizeof(Element*) * sufArray -> length);
  sufArray -> array = malloc(sizeof(int) * melody -> length);
  for (int i = 0; i < sufArray -> length; i++)
  {
    sufArray -> S[i] = melody -> element_array + i;
    sufArray -> array[i] = i;
  }

  //Ordeno el suffix array y lo retorno
  suffixArray_quickSort(sufArray, 0, sufArray -> length - 1);
  return sufArray;
}

/** Ordena el suffix array usando quicksort*/
void suffixArray_quickSort(SuffixArray* sufArray, int i, int j)
{
  //Caso base
  if (j - i <= 0) return;

  //Partition
  int p = partition(sufArray, i, j);

  //Llamada recursiva
  suffixArray_quickSort(sufArray, i, p - 1);
  suffixArray_quickSort(sufArray, p + 1, j);
}

/** Particiona el suffix array elgiendo como pivote el primer elemento*/
int partition(SuffixArray* sufArray, int i, int j)
{
  //Inicializo las variables
  int neg = i - 1;
  int piv = j;
  for (int k = i; k < piv; k++)
  {
    //Si es menor al pivote, hago swap
    if (cmp(sufArray, sufArray -> array[k], sufArray -> array[piv]) < 0)
    {
      swap(sufArray, neg + 1, k);
      neg++;
    }
  }
  //dejo el pivote almedio
  swap(sufArray, neg + 1, piv);
  return neg + 1;
}

/** Cambia en el sufArray el indice en la posicion a con el de la posicion b*/
void swap(SuffixArray* sufArray, int a, int b)
{
  int mem = sufArray -> array[a];
  sufArray -> array[a] = sufArray -> array[b];
  sufArray -> array[b] = mem;
}

/** Compara dos sufijos del sufArray en las posiciones i y j retorna -1,0,1*/
int cmp(SuffixArray* sufArray, int i, int j)
{
  //Si son iguales, retorno 0
  if (i == j) return 0;

  //Si i o j es el sufijo 0, solo comparo por duracion (no puede partir con silencio)
  Element e1, e2;
  e1 = sufArray -> S[i][0];
  e2 = sufArray -> S[j][0];
  if(e1.length != e2.length)
  {
    if (e1.length < e2.length) return -1;
    if (e1.length > e2.length) return 1;
  }
  //Mientras no se acabe ningun sufijo
  for (int a = 1; a < sufArray -> length - j && a < sufArray -> length - i; a++)
  {
    //Si un elemento es menor al otro, retorno, sino sigo al siguiente
    int comp = element_cmp(sufArray -> S[i][a], sufArray -> S[j][a]);
    if (comp != 0) return comp;
  }

  //digo que es mayor el mas largo de ambos
  if (i > j) return -1; //si i > j, entonces el sufijo i es mas corto
  return 1;
}

/** Crea un array de largo n-1 con la cantidad de elementos en comun de dos suficos ordenados consecutivos*/
int* suffixArray_common_elements(SuffixArray* sufArray)
{
  //Creo el arreglo de common
  int* common = malloc(sizeof(int) * (sufArray -> length - 1));

  //relleno el arreglo con los elementos en comun de un prefijo al otro
  for (int i = 0; i < sufArray -> length - 1; i++)
  {
    common[i] = prefix_common_elements(sufArray, sufArray -> array[i], sufArray -> array[i + 1]);
  }

  return common;
}

/** Calcula los elementos en comun entre los dos prefijos de i y j*/
int prefix_common_elements(SuffixArray* sufArray, int i, int j)
{
  // com guarda la cantidad de elementos en comun
  int com = 0;

  //Para el primer elemento no me importa la nota
  Element e1, e2;
  e1 = sufArray -> S[i][0];
  e2 = sufArray -> S[j][0];
  if (e1.type == NOTE && e2.type == NOTE && e1.length == e2.length) com++;

  //para el resto de los elementos hago la comparacion
  for (int a = 1; a < sufArray -> length - i && a < sufArray -> length - j; a++)
  {
    Element* els1 = sufArray -> S[i];
    Element* els2 = sufArray -> S[j];
    if (element_cmp(els1[a], els2[a]) == 0)
    {
      //sumo solo si es una nota
      if (els1[a].type == NOTE)
      {
        com++;
      }
    }
    //Si me encuentro un elemento distinto, dejo de buscar
    else
    {
      break;
    }
  }
  return com;
}

/** Guarda en melody la frase que mas se repite, retorna el puntero a la cancion completa*/
Element* find_frase(Melody* melody, SuffixArray* sufArray, int* common, int min_length)
{
  //Largo y cantidad seguidos actual
  int actual_length = 0;
  int actual_cuantity = 0;
  //Largo y cantidad seguidos record
  //max_length es la cantidad de notas (no silencios de la melody)
  int max_length = 0;
  int max_cuantity = 1;
  //Reprecentante de la secuencia y del maximo
  int actual_repr = 0;
  int max_repr = 0;
  for (int i = 0; i < sufArray -> length - 1; i++)
  {
    //Si supero el minimo
    if (common[i] >= min_length)
    {
      //Si estoy recien empesando
      if (actual_cuantity == 0)
      {
        //Hay dos melodias con el largo actual
        actual_cuantity = 2;
        //El largo es el valor de common
        actual_length = common[i];
        //El reprecentante es el que esta en la posicion i
        actual_repr = i;
      }
      //Si agrego una mas
      else
      {
        //Aumenta la cantidad de frases con string en comun
        actual_cuantity++;
        //El largo de la frase es el minimo entre el actual y el nuevo encontrado
        if (common[i] < actual_length) actual_length = common[i];
      }

      //Compruebo el maximo de cantidad, y en caso de empate, compruebo el maximo de largo
      if (actual_cuantity > max_cuantity || (actual_cuantity == max_cuantity && actual_length > max_length))
      {
        //Remplazo el valor del maximo de cantidad
        max_cuantity = actual_cuantity;
        //Remplazo el largo
        max_length = actual_length;
        //Remplazo el reprecentante
        max_repr = actual_repr;
      }
    }
    //Sino, actual es 0 para todo
    else
    {
      actual_length = 0;
      actual_cuantity = 0;
    }
  }
  // Finalmente max_repr me indica el indice (en arreglo de indices) de una de
  // las melodias que mas se repiten (es un reprecentante)


  // Remplazo en melody el puntero al arreglo de elementos, y cambio el largo
  // Y guardo el puntero anterior
  Element* mem = melody -> element_array;

	printf("Input Lenght: %i\n", min_length);
	printf("Cantidad de notas = %i\n", max_length);
	printf("Veces repetida = %i\n", max_cuantity);

	//Calculo el length en notas de la frace
  int length;
  if (max_cuantity == 1)
  {
    length = melody -> length;
  }
  else
  {
    melody -> element_array = sufArray -> S[sufArray -> array[max_repr]];
    length = 0;
    while (max_length > 0)
    {
      if(melody -> element_array[length].type == NOTE)
      {
        max_length--;
      }
      length++;
    }
  }
  melody -> length = length;

	printf("Cantidad de elementos %i\n", length);

  //Retorno el puntero al array completo (para poder liberarlo despues)
  return mem;
}

/** Libera memoria del suffix array*/
void suffixArray_destroy(SuffixArray* sufArray)
{
  free(sufArray -> S);
  free(sufArray -> array);
  free(sufArray);
}

/** Compara dos Element*/
int element_cmp(Element e1, Element e2)
{
  //Primero comparo si son nota o solencio
  if (e1.type < e2.type) return -1;
  if (e1.type > e2.type) return 1;
  //despues comparo el tono (solo si es nota)
  if (e1.type == NOTE)
  {
    if (e1.note < e2.note) return -1;
    if (e1.note > e2.note) return 1;
  }
  //despues comparo el largo
  if (e1.length < e2.length) return -1;
  if (e1.length > e2.length) return 1;
  return 0;
}

/** Imprime un Element*/
void element_print(Element el)
{
  if (el.type == NOTE)
  {
    printf("(%i,%i)", el.note, el.length);
  }
  else
  {
    printf("(S,%i)", el.length);
  }
}

/** Transforma un array de notas a un array de diferencias de notas*/
void melody_diff(Melody* melody)
{
  for (int i = melody -> length - 1; i > 0; i--)
  {
    Element* arr = melody -> element_array;
    arr[i].note = arr[i].note - arr[i-1].note;
  }
}

/** Transforma un array de diferencias de notas a un array de notas*/
void melody_undo_diff(Melody* melody)
{
  for (int i = 1; i < melody -> length; i++)
  {
    Element* arr = melody -> element_array;
    arr[i].note = arr[i].note + arr[i-1].note;
  }
}
