#ifndef T2_LIB_UTILES
#define T2_LIB_UTILES

#include "scene.h"
#include "stdio.h"

//Toma los datos de v2 y los pone en v1
void copiar_vector(Vector* v1, Vector* v2);

//Tomo el menor valor de cada coordenada de los vectores y los reemplazo en el primer vector
void minimizar_vector(Vector* vector1, Vector* vector2);

//Tomo el mayor valor de cada coordenada de ambos vectpres y los guardo en el vector 1
void maximizar_vector(Vector* vector1, Vector* vector2);


#endif
