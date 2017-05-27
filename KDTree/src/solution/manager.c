#include "manager.h"

/** Inicializa y configura el administrador de triángulos de la escena */
Manager* manager_init(Scene* scene)
{
  /* Solicitamos memoria para el manager */
  Manager* manager = malloc(sizeof(Manager));
  //Obtenemos los límites de los triángulos
  Triangle** triangulos = scene_get_triangles(scene);
  size_t triangle_count = scene_get_triangle_count(scene);
  Vector min_point;
  Vector max_point;
  Vector aux1, aux2, aux3;
  triangle_get_vertices(triangulos[0], &aux1, &aux2, &aux3);
  min_point = aux1;
  max_point = aux2;

  for (int i = 0; i < triangle_count; i++){
      triangle_get_vertices(triangulos[i], &aux1, &aux2, &aux3);
      minimizar_vector(&min_point, &aux1);
      minimizar_vector(&min_point, &aux2);
      minimizar_vector(&min_point, &aux3);
      maximizar_vector(&max_point, &aux1);
      maximizar_vector(&max_point, &aux2);
      maximizar_vector(&max_point, &aux3);
  }

  //creamos el kdtree
  manager -> kdtree = kdtree_init(triangulos, triangle_count, &min_point, &max_point, 0, 0, 0);
  return manager;
}

/** Encuentra el triangulo más cercano que intersecte con el rayo
    Retorna TRUE en caso de intersectar con algo, FALSE si no */
bool manager_get_closest_intersection(Manager* manager, Ray* ray)
{
    return kdtree_get_closest_intersection(manager -> kdtree, ray);
}

/** Libera todos los recursos asociados al administrador de triángulos */
void manager_destroy(Manager* manager)
{
  kdtree_destroy(manager -> kdtree);
  free(manager);
}
