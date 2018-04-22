#include "ANS/ANS.h"
#include "gurobi/gurobi.h"
#include <stdlib.h>
#include <stdio.h>
#include "random/pcg_basic.h"
#include <time.h>

void random_seed()
{
  double t1 = clock();
  double t2 = clock();
  pcg32_srandom((uint64_t)t1, (uint64_t)t2);
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
  ANS* ans = ans_init(argv[1], argv[2], argv[3]);

  ////////////////////////////////////////////////////////////////////////////
  //                                Tests                                   //
  ////////////////////////////////////////////////////////////////////////////

  // Creo un arreglo donde quepan rpa rutas de cada avion
  int rpa = 3;
  int count = rpa * airplanes_count;
  Route** routes = malloc(sizeof(Route*) * count);

  // itero sobre las aviones
  for (int k = 0; k < airplanes_count; k++)
  {
    printf("Avion %d\n", k);
    // 20 veces por avion
    for (int i = 0; i < rpa; i++)
    {
      printf("\tRuta %d\n", i);
      // Inicializo la ruta de esa avion y le aplico ANS
      printf("RUTA ORIGINAL\n");
      route_print(ans -> bp -> routes[k]);

      Route* copy = run(ans, ans -> bp -> routes[k]);
      printf("RUTA mejorada\n");
      assign_weights(copy);
      copy -> objective_function = objective_function(copy, ans -> map);
      route_print(copy);

      // Agrego esta ruta al arreglo
      routes[k * rpa + i] = copy;
    }
  }

  printf("///////// IMPRIMIENDO DENUEVO ///////////\n");
  for (int i = 0; i < count; i++)
  {
    route_print(routes[i]);
  }

  double* solution = optimize_routes(routes, count, ans -> map);

  if (solution)
  {
    for (int i = 0; i < count; i++)
    {
      printf("x_%d = %lf\n", i, solution[i]);
    }
  }

  ////////////////////////////////////////////////////////////////////////////
  //                           fin de los tests                             //
  ////////////////////////////////////////////////////////////////////////////

  return 0;
}
