#include "ANS/ANS.h"
#include "gurobi/gurobi.h"
#include <stdlib.h>
#include <stdio.h>
#include "random/pcg_basic.h"
#include <time.h>
#include <math.h>

// Le da una semilla random al generador de números aleatorios
void random_seed()
{
  double t1 = clock();
  double t2 = clock();
  pcg32_srandom((uint64_t)t1, (uint64_t)t2);
}

/** Retorna un randint entre 0 y bound */
int random_int(int bound)
{
  // Obtengo un int entre 0 y el maximo de ints
  uint32_t rand_int = pcg32_random();
  return rand_int % bound;
}

// Desordena la rutas (asume que es un arreglo con airplanes_count rutas)
void shuffle(Route** routes)
{
  for (int i = 0; i < airplanes_count - 1; i++)
  {
    int step = random_int(airplanes_count - i);
    size_t j = i + step;
    Route* aux = routes[j];
    routes[j] = routes[i];
    routes[i] = aux;
  }
}

// Crea rutas vacias para cada avion
Route** create_empty_routes(ANS* ans)
{
  // Creo una ruta vacia para cada avion
  Route** solution = malloc(sizeof(Route*) * airplanes_count);

  // Inicializo las rutas
  for (int k = 0; k < airplanes_count; k++)
  {
    // Copio las rutas de la planificacion base
    solution[k] = route_copy(ans -> bp -> routes[k], ans -> map);
    // Mientras queden nodos de pickup y delivery
    while (solution[k] -> nodes -> start -> next -> node -> node_type != END)
    {
      // Elimino el primer pedido
      l_node_pop(solution[k], solution[k] -> nodes -> start -> next);
    }
    // Ahora es una ruta vacia
    solution[k] -> objective_function = objective_function(solution[k], ans -> map);
    solution[k] -> fast_of = fast_of(solution[k], ans -> map);
  }

  return solution;
}

// Crea las rutas iniciales para cada avion
Route** create_initial_routes(ANS* ans)
{
  // Obtengo una ruta vacia para cada avion (solo start y end)
  Route** routes = create_empty_routes(ans);

  // Creo un arreglo con todos los pedidos (arreglo de pedidos pendientes)
  int unnasigned = order_count;
  Order** unassigned_orders = malloc(sizeof(Order*) * order_count);
  // Lo relleno con todos los pedidos
  for (int i = 0; i < order_count; i++)
  {
    unassigned_orders[i] = ans -> map -> orders[i];
  }

  // Itero agregando pedidos a las rutas
  while (true)
  {
    // Desordena las rutas
    shuffle(routes);

    // Cuenta a cuantas rutas no se les pudo insertar ningun pedido
    int fails = 0;
    // Itero sobre las rutas
    for (int i = 0; i < airplanes_count; i++)
    {
      // Por ahora estos son los mejores pedidos a insertar
      Order* best = NULL;
      int best_id = 0;
      double best_cost = INFINITY;

      // Creo el l_nodo a insertar en la ruta
      LNode* pickup = l_node_init();
      // Ontengo el delivery
      LNode* delivery = pickup -> pair;

      // itero sobre los pedidos
      for (int p = 0; p < unnasigned; p++)
      {
        // Obtengo la posicion a insertar el nodo (Al final)
        LNode* position = routes[i] -> nodes -> end -> last;

        // Relleno el l_nodo con el pedido
        l_node_order_fill(pickup, unassigned_orders[p]);

        // Inserto el pedido al final
        l_node_insert_next(routes[i], position, pickup);
        l_node_insert_next(routes[i], pickup, delivery);

        // Calculo el costo de la insercion (solo costos de viaje)
        double cost = 0;
        cost += distance(position -> node -> father, pickup -> node -> father);
        cost += distance(pickup -> node -> father, delivery -> node -> father);
        cost += distance(delivery -> node -> father, delivery -> next -> node -> father);

        // Si la ruta es valida
        if (assign_time(routes[i]) && assign_weights(routes[i]))
        {
          // Si mejore el costo
          if (cost < best_cost)
          {
            // Actualizo los mejores valores
            best = unassigned_orders[p];
            best_cost = cost;
            best_id = p;
          }
        }

        // Deshago la insercion
        l_node_pop(routes[i], delivery);
        l_node_pop(routes[i], pickup);
      }

      // Si pude insertar algun nodo
      if (best)
      {
        // Inserto el pedido al final
        l_node_order_fill(pickup, best);
        LNode* position = routes[i] -> nodes -> end -> last;
        l_node_insert_next(routes[i], position, pickup);
        l_node_insert_next(routes[i], pickup, delivery);

        // Elimino el pedido de los pendientes
        for (int j = best_id; j < unnasigned - 1; j++)
        {
          // Muevo los pedidos para rellenar el hueco
          unassigned_orders[j] = unassigned_orders[j + 1];
        }
        // Ahora hay un pedido menos
        unnasigned--;
      }
      // Si no pude insertar
      else
      {
        // Cuento un fail mas
        fails++;
        // Libero la memoria de los l_nodos
        free(pickup);
        free(delivery);
      }
    }

    // Si no me quedan pedidos por asignar o no pude asirgnar pedidos a ninguna
    // ruta, termino de iterar
    if (fails == airplanes_count || unnasigned == 0)
    {
      break;
    }
  }

  // Dejo los valores de las rutas correctos
  for (int i = 0; i < airplanes_count; i++)
  {
    assign_time(routes[i]);
    assign_weights(routes[i]);
    routes[i] -> objective_function = objective_function(routes[i], ans -> map);
    routes[i] -> fast_of = fast_of(routes[i], ans -> map);
  }

  Route** sorted = malloc(sizeof(Route*) * airplanes_count);

  for (int i = 0; i < airplanes_count; i++)
  {
    sorted[routes[i] -> airplane -> id] = routes[i];
  }

  free(routes);

  return sorted;
}

// Crea muchas columnas iniciales
void generate_initial_routes(ANS* ans, int airplane_id, Route* initial_route)
{
  // Insertare las rutas en ans -> routes[airplane_id]
  Route** routes = ans -> routes[airplane_id];
  int position = 0;

  // R1
  printf("\t\tR1\n");

  // La primera ruta es copia de la planificacion base eliminando pedidos de costo 0
  Route* r1 = route_copy(ans -> bp -> routes[airplane_id], ans -> map);

  // Ajusto sus cargas
  assign_weights(r1);

  // Itero eliminando pedidos con costo 0
  LNode* actual = r1 -> nodes -> start;
  while (actual -> next != r1 -> nodes -> end)
  {
    LNode* next = actual -> next;
    if (next -> delta_weight == 0)
    {
      l_node_pop(r1, next);
      l_node_pop(r1, next -> pair);
    }
    else
    {
      actual = actual -> next;
    }
  }

  // Si es una ruta valida
  bool destroy_r1 = false;
  if (assign_time(r1) && assign_weights(r1))
  {
    // Agrego esta ruta
    routes[position] = r1;
    position++;
    r1 -> valid = true;
  }
  else
  {
    destroy_r1 = true;
  }


  // R2
  printf("\t\tR2\n");

  // Tomo la ruta anterior y le hago drop_and_add
  Route* r2 = initial_drop_and_add(r1, ans);


  // Si es una ruta valida
  bool destroy_r2 = false;
  if (assign_time(r2) && assign_weights(r2))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r2 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r2;
      position++;
      r2 -> valid = true;
    }
    else
    {
      destroy_r2 = true;
    }
  }
  else
  {
    destroy_r2 = true;
  }

  // R3
  printf("\t\tR3\n");

  // Le hago swap (especial)
  Route* r3 = initial_swap(r2, ans);

  // Si es una ruta valida
  if (assign_time(r3) && assign_weights(r3))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r3 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r3;
      position++;
      r3 -> valid = true;
    }
    else
    {
      route_destroy(r3);
    }
  }
  else
  {
    route_destroy(r3);
  }

  if (destroy_r1) route_destroy(r1);
  if (destroy_r2) route_destroy(r2);

  // R4
  printf("\t\tR4\n");

  // Tomo la ruta vacia (solo inicio y fin)
  Route* r4 = route_copy(ans -> bp -> routes[airplane_id], ans -> map);
  // Mientras queden nodos de pickup y delivery
  while (r4 -> nodes -> start -> next -> node -> node_type != END)
  {
    // Elimino el primer pedido
    l_node_pop(r4, r4 -> nodes -> start -> next);
  }
  // Ahora es una ruta vacia
  r4 -> objective_function = objective_function(r4, ans -> map);
  r4 -> fast_of = fast_of(r4, ans -> map);

  // Si es una ruta valida
  if (assign_time(r4) && assign_weights(r4))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r4 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r4;
      position++;
      r4 -> valid = true;
    }
    else
    {
      route_destroy(r4);
    }
  }
  else
  {
    route_destroy(r4);
  }

  // R5
  printf("\t\tR5\n");

  // Ocupo la ruta generada aleatoria de la funcion create_initial_routes
  Route* r5 = route_copy(initial_route, ans -> map);

  // Si es una ruta valida
  bool destroy_r5 = false;
  if (assign_time(r5) && assign_weights(r5))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r5 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r5;
      position++;
      r5 -> valid = true;
    }
    else
    {
      destroy_r5 = true;
    }
  }
  else
  {
    destroy_r5 = true;
  }

  // R6
  printf("\t\tR6\n");

  // Tomo la ruta anterior y le hago drop_and_add y luego swap
  Route* r6 = initial_drop_and_add(r5, ans);

  // Si es una ruta valida
  bool destroy_r6 = false;
  if (assign_time(r6) && assign_weights(r6))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r6 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r6;
      position++;
      r6 -> valid = true;
    }
    else
    {
      destroy_r6 = true;
    }
  }
  else
  {
    destroy_r6 = true;
  }

  // R7
  printf("\t\tR7\n");
  Route* r7 = initial_swap(r6, ans);

  // Si es una ruta valida
  if (assign_time(r7) && assign_weights(r7))
  {
    bool repeated = false;
    // Si no tiene valores iguales a una anterior
    for (int i = 0; i < position; i++)
    {
      if (r7 -> objective_function == routes[i] -> objective_function)
      {
        repeated = true;
        break;
      }
    }
    if (!repeated)
    {
      routes[position] = r7;
      position++;
      r7 -> valid = true;
    }
    else
    {
      route_destroy(r7);
    }
  }
  else
  {
    route_destroy(r7);
  }

  if (destroy_r5) route_destroy(r5);
  if (destroy_r6) route_destroy(r6);

  // Actualizo la cantidad de rutas que tiene el sp del avion actual
  ans -> route_count[airplane_id] = position;
}

// Elige una ruta aleatoria del arreglo y la retorna
Route* choose_random(Route** routes, int count)
{
  int position = random_int(count);
  return routes[position];
}

void recalculate_of(ANS* ans)
{
  // Itero sobre las avions
  for (int k = 0; k < airplanes_count; k++)
  {
    // Itero por las rutas
    for (int i = 0; i < ans -> route_count[k]; i++)
    {
      ans -> routes[k][i] -> objective_function = objective_function(ans -> routes[k][i], ans -> map);
      ans -> routes[k][i] -> fast_of = fast_of(ans -> routes[k][i], ans -> map);
    }
  }
}

// Loop principal del programa
double* solve(ANS* ans)
{
  // creo las rutas iniciales de cada avion
  printf("\tGenerando rutas\n");
  Route** routes_initial = create_initial_routes(ans);

  printf("\tGenerando mas columnas para cada avion\n");
  // Genero mas columnas
  clock_t start = clock();
  for (int i = 0; i < airplanes_count; i++)
  {
    printf("\t\tAvion %d\n", i);
    // route_print(routes_initial[i]);
    generate_initial_routes(ans, i, routes_initial[i]);
    // Libero memoria
    route_destroy(routes_initial[i]);
  }
  free(routes_initial);
  clock_t end = clock();

  printf("\tTiempo en generar columnas iniciales: %lf\n", (double)(end - start)/CLOCKS_PER_SEC);

  printf("\tLoop principal:\n");
  // Variable para contar iteraciones
  int iteration = 0;
  // Itero hasta que no genere nuevas columnas buenas
  while (true)
  {
    printf("\t\tIteracion %d\n", iteration);
    // Resuelvo el problema maestro relajado y actualizo los duales de las rutas
    printf("\t\tResolviendo problema maestro relajado\n");
    optimize_routes_relaxed(ans -> routes, ans -> route_count, ans -> map);

    // Recalculo la funcion objetivo de todas las rutas
    printf("\t\tRecalculando funciones objetivo\n");
    recalculate_of(ans);

    // Cuento cuantas rutas con costo reducido mayor a 0 produje
    int improved_count = 0;
    // Para cada avion
    printf("\t\tEjecutar ANS\n");
    for (int k = 0; k < airplanes_count; k++)
    {
      printf("\t\t\tAvion %d\n", k);
      // Elijo una ruta aleatoria
      Route* route = choose_random(ans -> routes[k], ans -> route_count[k]);

      // Le aplico la heuristica
      Route* new_route = run(ans, route);

      // Si su costo reducido es mayor a 0, la agrego al arreglo de rutas
      if (new_route -> objective_function > 0)
      {
        insert_route(ans, new_route, k);
      }

      // Cuento una ruta con costo reducido mayor a 0
      improved_count++;
    }

    // Agrego 1 a la cuenta de iteraciones
    iteration++;

    printf("\t\tSe produjieron %d rutas con costo reducido positivo\n", improved_count);

    // Si no produje ninguna ruta con costo reducido mayor a 0, termino
    if (improved_count == 0) break;
  }

  printf("\tEjecutando problema maestro restringido\n");
  // Resuelvo el problema maestro con variables binarias
  double* solution = optimize_routes(ans -> routes, ans -> route_count, ans -> map);

  printf("\tRetornando solucion\n");
  // Retorno la solucion
  return solution;
}

int main(int argc, char *argv[])
{
  if (argc == 4)
  {
    optimizing_weights = false;
  }
  else if (argc == 5)
  {
    optimizing_weights = true;
  }
  else
  {
    printf("Modo de uso:\n");
    printf("[./optimizador] [map_file] [airplanes_file] [BP_file] (-ow)\n");
    exit(0);
  }

  // Pongo una seed aleatoria al random del programa
  random_seed();

  // Inicializo el ans
  printf("Leyendo archivos\n");
  ANS* ans = ans_init(argv[1], argv[2], argv[3]);


  ////////////////////////////////////////////////////////////////////////////
  //                                Tests                                   //
  ////////////////////////////////////////////////////////////////////////////

  printf( "Ejecutando iteracion 0\n");
  clock_t start = clock();
  double* solution = solve(ans);
  clock_t end = clock();
  printf( "Tiempo de solve = %lf\n", (double)(end - start) / CLOCKS_PER_SEC);


  return 0;
}
