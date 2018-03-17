#include "ANS/ANS.h"
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
  if (argc != 4)
  {
    printf("Modo de uso:\n");
    printf("[./optimizador] [map_file] [airplanes_file] [BP_file]\n");
  }

  // Pongo una seed aleatoria al random del programa
  random_seed();

  // Inicializo el ans
  ANS* ans = ans_init(argv[1], argv[2], argv[3]);

  ////////////////////////////////////////////////////////////////////////////
  //                                Tests                                   //
  ////////////////////////////////////////////////////////////////////////////

  

  ////////////////////////////////////////////////////////////////////////////
  //                           fin de los tests                             //
  ////////////////////////////////////////////////////////////////////////////

  return 0;
}
