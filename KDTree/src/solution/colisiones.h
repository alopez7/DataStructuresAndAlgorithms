#ifndef T2_LIB_COLISIONES
#define T2_LIB_COLISIONES

#include "stdio.h"
#include "utiles.h"

//Retorna la distancia en que colisiona con e plano, -1 si no colisiona
float colision_rayo_plano(Ray* rayo, Vector* min, Vector* max, int eje);

//Retorna la distancia ocn que colisiona con el plano, -1 si no colisiona
float colision_rayo_caja(Ray* rayo, Vector* lim_min, Vector* lim_max);

//0 si no choca, 1 si choca con la priemra caja, 2 si choca con la segunda, 3 si choca con la primera y luego la segunda
//4 si choca con la segunda y luego la primera
int orden_choque(Ray* rayo, Vector* lim_min_izq, Vector* lim_max_izq, Vector* lim_min_der, Vector* lim_max_der);


#endif
