#include "kdtree.h"



//inicio el kdtree
//En este inicio se crean los hijos recursivamente
Kdtree* kdtree_init(Triangle** triangles, int triangle_count, Vector* min_point, Vector* max_point, int plano, float limite, int lad){


  //Solicitamos memoria para el kdtree
  Kdtree* kdtree = malloc(sizeof(Kdtree));


  //SETEAR VALORES TRIANGLES, TRIANGLE_COUNT, PLANO, LIMITE, LADO
  kdtree -> triangle_count = triangle_count;
  kdtree -> triangles = triangles;
  kdtree -> plano = plano;
  kdtree -> limite = limite;
  kdtree -> lado = lad;


  //SETEAR LAS DIMENSIONES
  //Reservo memoria para los vectores min_point y max_point
  kdtree -> min_point = malloc(sizeof(Vector));
  kdtree -> max_point = malloc(sizeof(Vector));
  //Copios los valores del mínimo y del máximo
  copiar_vector(kdtree -> min_point, min_point);
  copiar_vector(kdtree -> max_point, max_point);
  //Obtengo los límites de los triángulos para achicar la caja
  //creo las variables para guardar los vértices de los triángulos
  Vector aux1, aux2, aux3;
  for (int i = 0; i < triangle_count; i++){
      //obtengo los vértices de los triángulos y los guardo en las variables creadas
      triangle_get_vertices(triangles[i], &aux1, &aux2, &aux3);
      //veo si es nuevo máximo o mínimo
      minimizar_vector(kdtree -> min_point, &aux1);
      minimizar_vector(kdtree -> min_point, &aux2);
      minimizar_vector(kdtree -> min_point, &aux3);
      maximizar_vector(kdtree -> max_point, &aux1);
      maximizar_vector(kdtree -> max_point, &aux2);
      maximizar_vector(kdtree -> max_point, &aux3);
  }
  //minimizo el máximo y maximizo el mínimo
  maximizar_vector(kdtree -> min_point, min_point);
  minimizar_vector(kdtree -> max_point, max_point);


  //CREAR HIJOS
  //Creo los vectores nuevos (Límites) de los hijos
  Vector lim_min_izq;
  Vector lim_max_izq;
  Vector lim_min_der;
  Vector lim_max_der;
  //obtengo el lado más largo y creo los límites de las cajas pequeñas
  float dif_x = kdtree -> max_point -> X - kdtree -> min_point -> X;
  float dif_y = kdtree -> max_point -> Y - kdtree -> min_point -> Y;
  float dif_z = kdtree -> max_point -> Z - kdtree -> min_point -> Z;
  int lado_cortado; // 1 si es x, 2 si es y, 3 si es z
  float valor; //distancia del plano cortador del origen
  if (dif_x >= dif_y && dif_x >= dif_z){
      lado_cortado = 1;
      valor = (kdtree -> max_point -> X + kdtree -> min_point -> X) / 2;
      copiar_vector(&lim_max_izq, max_point);
      lim_max_izq.X = valor;
      copiar_vector(&lim_min_der, min_point);
      lim_min_der.X = valor;
  }
  else if (dif_y >= dif_x && dif_y >= dif_z){
      lado_cortado = 2;
      valor = (kdtree -> max_point -> Y + kdtree -> min_point -> Y) / 2;
      copiar_vector(&lim_max_izq, max_point);
      lim_max_izq.Y = valor;
      copiar_vector(&lim_min_der, min_point);
      lim_min_der.Y = valor;
  }
  else {
      lado_cortado = 3;
      valor = (kdtree -> max_point -> Z + kdtree -> min_point -> Z) / 2;
      copiar_vector(&lim_max_izq, max_point);
      lim_max_izq.Z = valor;
      copiar_vector(&lim_min_der, min_point);
      lim_min_der.Z = valor;
  }
  copiar_vector(&lim_min_izq, min_point);
  copiar_vector(&lim_max_der, max_point);



  //Cuento cuantos triángulos hay en cada hijo
  int contador_izq = 0;
  int contador_der = 0;
  //Guardo 1 si está en la primera caja, 2 si está en la segunda y 3 si está en ambas
  int* posicion = malloc(sizeof(int) * triangle_count);
  //Los llevo todos a 0
  for (int i = 0; i < triangle_count; i++){
      posicion[i] = 0;
  }
  int lado;
  for (int i = 0; i < triangle_count; i++){
      //Retorna la mitad donde está el triángulo
      lado = lado_del_plano(lado_cortado, valor, triangles[i]);
      if (lado == 1){
          contador_izq = contador_izq + 1;
          posicion[i] = 1;
      }
      else if (lado == 2){
          contador_der = contador_der + 1;
          posicion[i] = 2;
      }
      else {
          contador_izq = contador_izq + 1;
          contador_der = contador_der + 1;
          posicion[i] = 3;
      }
  }



  //Veo si me conviene subdividir o no
  //Si la cantidad de triángulos que están en ambas cajas es mayor o igual al 50%, no subdivido
  //Y si la cantidad de tripangulos es mayor menor a 10, tampoco subdivido
  if (contador_izq + contador_der < 1.3 * triangle_count && triangle_count > 20){

      //SETEO LA VARIABLE LEAF
      kdtree -> leaf = false;
      //Creo arreglos para guardar los triangulos de cada hijo
      Triangle** triangles_left = malloc(sizeof(Triangle*) * contador_izq);
      int contador_left = 0;
      Triangle** triangles_right = malloc(sizeof(Triangle*) * contador_der);
      int contador_right = 0;
      //agrego los triangulos a los arreglos
      for (int i = 0; i < triangle_count; i++){
          if (posicion[i] == 1){
              triangles_left[contador_left] = triangles[i];
              contador_left = contador_left + 1;
          }
          else if (posicion[i] == 2){
              triangles_right[contador_right] = triangles[i];
              contador_right = contador_right + 1;
          }
          else if (posicion[i] == 3){
              triangles_left[contador_left] = triangles[i];
              contador_left = contador_left + 1;
              triangles_right[contador_right] = triangles[i];
              contador_right = contador_right + 1;
          }
      }
      //Creo hijos y los inicio
      kdtree -> left_node = kdtree_init(triangles_left, contador_izq, &lim_min_izq, &lim_max_izq, lado_cortado, valor, 1);
      kdtree -> right_node = kdtree_init(triangles_right, contador_der, &lim_min_der, &lim_max_der, lado_cortado, valor, 2);
      if (kdtree -> lado != 0){
        free(triangles);
      }

  }
  else {
      //Seteo variable leaf
      kdtree -> leaf = true;
  }
  free(posicion);
  return kdtree;
}

//Retorna 0 si hay un error, 1 si es menor al plano, 3 si es mayor y menor al plano y 2 si es mayor al plano
int lado_del_plano(int eje, float valor, Triangle* triangulo){
    //obtengo los vértices del triángulo
    Vector aux1, aux2, aux3;
    triangle_get_vertices(triangulo, &aux1, &aux2, &aux3);
    bool positivo = false;
    bool negativo = false;

    //obtengo el valor del eje a comparar
    float comp1;
    float comp2;
    float comp3;
    if (eje == 1){
        comp1 = aux1.X;
        comp2 = aux2.X;
        comp3 = aux3.X;
    }
    else if (eje == 2){
        comp1 = aux1.Y;
        comp2 = aux2.Y;
        comp3 = aux3.Y;
    }
    else if (eje == 3){
        comp1 = aux1.Z;
        comp2 = aux2.Z;
        comp3 = aux3.Z;
    }
    else {
        return 0;
    }

    //comparo con el plano
    if (comp1 <= valor || comp2 <= valor || comp3 <= valor){
        negativo = true;
    }
    if (comp1 >= valor || comp2 >= valor || comp3 >= valor){
        positivo = true;
    }

    //veo como está el triángulo respecto al plano
    if (negativo && positivo){
        return 3;
    }
    if (negativo){
        return 1;
    }
    if (positivo){
        return 2;
    }
    return 0;
}

//Retorna true si el triángulo no está en la caja, y false en caso contrario
bool no_en_caja(Triangle* triangulo, Kdtree* kdtree){
    Vector aux1, aux2, aux3;
    triangle_get_vertices(triangulo, &aux1, &aux2, &aux3);
    float lim = kdtree -> limite;
    if (kdtree -> plano == 1){
        if (kdtree -> lado == 1){
            if (aux1.X <= lim || aux2.X <= lim || aux3.X <= lim){
                return false;
            }
        }
        else if (kdtree -> lado == 2){
            if (aux1.X >= lim || aux2.X >= lim || aux3.X >= lim){
                return false;
            }
        }
        else {
            return true;
        }
    }
    else if (kdtree -> plano == 2){
        if (kdtree -> lado == 1){
            if (aux1.Y <= lim || aux2.Y <= lim || aux3.Y <= lim){
                return false;
            }
        }
        else if (kdtree -> lado == 2){
            if (aux1.Y >= lim || aux2.Y >= lim || aux3.Y >= lim){
                return false;
            }
        }
        else {
            return true;
        }
    }
    else if(kdtree -> plano == 3){
        if (kdtree -> lado == 1){
            if (aux1.Z <= lim || aux2.Z <= lim || aux3.Z <= lim){
                return false;
            }
        }
        else if (kdtree -> lado == 2){
            if (aux1.Z >= lim || aux2.Z >= lim || aux3.Z >= lim){
                return false;
            }
        }
        else {
            return true;
        }
    }
    return false;
}


/** Encuentra el triangulo más cercano que intersecte con el rayo
    Retorna TRUE en caso de intersectar con algo, FALSE si no */
bool kdtree_get_closest_intersection(Kdtree* kdtree, Ray* ray) {
    Triangle* anterior = ray_get_intersected_object(ray);
    if (anterior != NULL){
        if (kdtree -> leaf){
            //Recorro el arreglo de triángulos
            for (int i = 0; i < kdtree -> triangle_count; i++)
            {
              //Veo si se intersecta con el triángulo
              ray_intersect(ray, kdtree -> triangles[i]);
            }
            // Si luego de todo eso hay un triangulo guardado en el rayo
            Triangle* nuevo = ray_get_intersected_object(ray);
            if (nuevo == anterior){
                return false;
            }
            return true;
        }

        // 0 si no choca ninguna caja, 1 si choca solo la primera, 2 si choca solo la segunda
        // 3 si choca primero la primera y luego la segunda, 4 si choca primero la segunda y luego la primera
        int orden = orden_choque(ray, kdtree -> left_node -> min_point, kdtree -> left_node -> max_point,
                                kdtree -> right_node -> min_point, kdtree -> right_node -> max_point);
        //Casos a seguir
        if (orden == 0){
            return false;
        }
        else if (orden == 1){
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            Triangle* nuevo = ray_get_intersected_object(ray);
            if (nuevo != anterior){
                return true;
            }
            return false;
        }
        else if (orden == 2){
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            Triangle* nuevo = ray_get_intersected_object(ray);
            if (nuevo != anterior){
                return true;
            }
            return false;
        }
        else if (orden == 3){
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            Triangle* nuevo = ray_get_intersected_object(ray);
            if (nuevo != anterior){
                if (no_en_caja(nuevo, kdtree -> right_node)){
                    return true;
                }
            }
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            Triangle* nuevisimo = ray_get_intersected_object(ray);
            if (nuevisimo != nuevo){
                return true;
            }
            return false;
        }
        else {
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            Triangle* nuevo = ray_get_intersected_object(ray);
            if (nuevo != anterior){
                if (no_en_caja(nuevo, kdtree -> left_node)){
                    return true;
                }
            }
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            Triangle* nuevisimo = ray_get_intersected_object(ray);
            if (nuevisimo != nuevo){
                return true;
            }
            return false;
        }
    }
    else { //Si no tiene ninguna interseccion anterior
        if (kdtree -> leaf){
            //Recorro el arreglo de triángulos
            for (int i = 0; i < kdtree -> triangle_count; i++)
            {
                //Veo si se intersecta con el triángulo
                ray_intersect(ray, kdtree -> triangles[i]);
            }
            // Si luego de todo eso hay un triangulo guardado en el rayo
            if(ray_get_intersected_object(ray) != NULL){
                return true;
            }
            return false;
        }

        // 0 si no choca ninguna caja, 1 si choca solo la primera, 2 si choca solo la segunda
        // 3 si choca primero la primera y luego la segunda, 4 si choca primero la segunda y luego la primera
        int orden = orden_choque(ray, kdtree -> left_node -> min_point, kdtree -> left_node -> max_point,
                                kdtree -> right_node -> min_point, kdtree -> right_node -> max_point);
        //Casos a seguir
        if (orden == 0){
            return false;
        }
        else if (orden == 1){
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            if (ray_get_intersected_object(ray) != NULL){
                return true;
            }
            return false;
        }
        else if (orden == 2){
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            if (ray_get_intersected_object(ray) != NULL){
                return true;
            }
            return false;
        }
        else if (orden == 3){
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            Triangle* triangulo = ray_get_intersected_object(ray);
            if (triangulo != NULL){
                if (no_en_caja(triangulo, kdtree -> right_node)){
                    return true;
                }
            }
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            if (ray_get_intersected_object(ray) != NULL){
                return true;
            }
            return false;
        }
        else {
            kdtree_get_closest_intersection(kdtree -> right_node, ray);
            Triangle* triangulo = ray_get_intersected_object(ray);
            if (triangulo != NULL){
                if (no_en_caja(triangulo, kdtree -> left_node)){
                    return true;
                }
            }
            kdtree_get_closest_intersection(kdtree -> left_node, ray);
            if (ray_get_intersected_object(ray) != NULL){
                return true;
            }
            return false;
        }
    }
}


/** Libera todos los recursos asociados al administrador de triángulos  de forma recursiva*/
void kdtree_destroy(Kdtree* kdtree)
{
  if (!kdtree -> leaf){
      kdtree_destroy(kdtree -> left_node);
      kdtree_destroy(kdtree -> right_node);
  }
  else {
      if (kdtree -> lado != 0){
        free(kdtree -> triangles);
      }
  }
  free(kdtree -> max_point);
  free(kdtree -> min_point);
  free(kdtree);
}
