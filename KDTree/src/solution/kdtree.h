#ifndef T2_LIB_KDTREE
#define T2_LIB_KDTREE

#include <stdbool.h>
#include "colisiones.h"



struct kdtree
{
  Triangle** triangles;
  int triangle_count;
  Vector* min_point;
  Vector* max_point;
  struct kdtree* left_node;
  struct kdtree* right_node;
  bool leaf;
  int plano;//1 = x, 2 = y, 3 = z, 0 = nodo original
  float limite;//Valor del plano divisor
  int lado;//1 si es menor al plano 2 si es mayor al plano
};
/** Representa la estructura encargada de administrar los triángulos */
typedef struct kdtree Kdtree;

/** Inicializa y configura el administrador de triángulos de la escena */
Kdtree* kdtree_init(Triangle** triangles, int triangle_count, Vector* min_point, Vector* max_point, int plano, float limite, int lado);

/** Retorna 1 si está a la izquierda, 2 si está a la derecha, y 3 si está a ambos lados**/
int      lado_del_plano(int eje, float valor, Triangle* triangulo);

/** Retorna true si el triángulo no está en la caja, y false en caso contrario **/
bool     no_en_caja(Triangle* triangulo, Kdtree* kdtree);

/** Encuentra el triangulo más cercano que intersecte con el rayo
    Retorna TRUE en caso de intersectar con algo, FALSE si no */
bool     kdtree_get_closest_intersection(Kdtree* kdtree, Ray* ray);

/** Libera todos los recursos asociados al administrador de triángulos */
void     kdtree_destroy(Kdtree* kdtree);

#endif
