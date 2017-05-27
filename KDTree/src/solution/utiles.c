#include "utiles.h"
#include "stdio.h"


//Toma los datos de v2 y los pone en v1
void copiar_vector(Vector* v1, Vector* v2){
    v1 -> X = v2 -> X;
    v1 -> Y = v2 -> Y;
    v1 -> Z = v2 -> Z;
}

//Tomo el menor valor de cada coordenada de los vectores y los reemplazo en el primer vector
void minimizar_vector(Vector* vector1, Vector* vector2){
    if (vector1 -> X > vector2 -> X){
        vector1 -> X = vector2 -> X;
    }
    if (vector1 -> Y > vector2 -> Y){
        vector1 -> Y = vector2 -> Y;
    }
    if (vector1 -> Z > vector2 -> Z){
        vector1 -> Z = vector2 -> Z;
    }
}

//Tomo el mayor valor de cada coordenada de ambos vectpres y los guardo en el vector 1
void maximizar_vector(Vector* vector1, Vector* vector2){
    if (vector1 -> X < vector2 -> X){
        vector1 -> X = vector2 -> X;
    }
    if (vector1 -> Y < vector2 -> Y){
        vector1 -> Y = vector2 -> Y;
    }
    if (vector1 -> Z < vector2 -> Z){
        vector1 -> Z = vector2 -> Z;
    }
}
