#ifndef T2_LIB_MANAGER
#define T2_LIB_MANAGER

#include <stdbool.h>
#include "kdtree.h"

/* TODO Debes modificar este struct para implementar tu estructura */
struct manager
{
  //El manager tiene asociado al padre del árbol (el nodo de más arriba)
  Kdtree* kdtree;
};
/** Representa la estructura encargada de administrar los triángulos */
typedef struct manager Manager;

/** Inicializa y configura el administrador de triángulos de la escena */
Manager* manager_init(Scene* scene);

/** Encuentra el triangulo más cercano que intersecte con el rayo
    Retorna TRUE en caso de intersectar con algo, FALSE si no */
bool     manager_get_closest_intersection(Manager* manager, Ray* ray);

/** Libera todos los recursos asociados al administrador de triángulos */
void     manager_destroy(Manager* manager);

#endif /* end of include guard: T2_LIB_STRUCTURE */
