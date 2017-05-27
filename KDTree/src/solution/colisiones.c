#include "colisiones.h"

//retorna la distancia en que colisiona con el plano (-1 si no colisiona)
//eje es 1 si es X, 2 si es Y, 3 si es Z
float colision_rayo_plano(Ray* rayo, Vector* min, Vector* max, int eje){
    float distancia = -1;
    Vector pos = ray_get_origin(rayo);
    Vector dir = ray_get_direction(rayo);
    if (eje == 1){
        distancia = (min -> X - pos.X)/dir.X;
        if (distancia < 0){
            return -1;
        }
        float Y = pos.Y + distancia * dir.Y;
        float Z = pos.Z + distancia * dir.Z;
        if (Y < min -> Y || Y > max -> Y || Z < min -> Z || Z > max -> Z){
            return -1;
        }
    }
    else if (eje == 2){
        distancia = (min -> Y - pos.Y)/dir.Y;
        if (distancia < 0){
            return -1;
        }
        float X = pos.X + distancia * dir.X;
        float Z = pos.Z + distancia * dir.Z;
        if (X < min -> X || X > max -> X || Z < min -> Z || Z > max -> Z){
            return -1;
        }
    }
    else {
        distancia = (min -> Z - pos.Z)/dir.Z;
        if (distancia < 0){
            return -1;
        }
        float Y = pos.Y + distancia * dir.Y;
        float X = pos.X + distancia * dir.X;
        if (Y < min -> Y || Y > max -> Y || X < min -> X || X > max -> X){
            return -1;
        }
    }
    return distancia;
}

//retorna -1 si no choca a la caja o si la choca hacia el otro lado, retorna la distancia en que lo choca en otro caso
float colision_rayo_caja(Ray* rayo, Vector* lim_min, Vector* lim_max){
    float distancia = -1;
    float dist;
    Vector min1, min2, min3, min4, min5, min6;
    Vector max1, max2, max3, max4, max5, max6;
    copiar_vector(&min1, lim_min);
    copiar_vector(&max1, lim_max);
    max1.Z = lim_min -> Z;
    dist = colision_rayo_plano(rayo, &min1, &max1, 3);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    copiar_vector(&min2, lim_min);
    copiar_vector(&max2, lim_max);
    max2.Y = lim_min -> Y;
    dist = colision_rayo_plano(rayo, &min2, &max2, 2);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    copiar_vector(&min3, lim_min);
    copiar_vector(&max3, lim_max);
    max3.X = lim_min -> X;
    dist = colision_rayo_plano(rayo, &min3, &max3, 1);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    copiar_vector(&min4, lim_min);
    min4.X = lim_max -> X;
    copiar_vector(&max4, lim_max);
    dist = colision_rayo_plano(rayo, &min4, &max4, 1);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    copiar_vector(&min5, lim_min);
    min5.Y = lim_max -> Y;
    copiar_vector(&max5, lim_max);
    dist = colision_rayo_plano(rayo, &min5, &max5, 2);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    copiar_vector(&min6, lim_min);
    min6.Z = lim_max -> Z;
    copiar_vector(&max6, lim_max);
    dist = colision_rayo_plano(rayo, &min6, &max6, 3);
    if (dist != -1){
        if (dist < distancia || distancia == -1){
            distancia = dist;
        }
    }
    return distancia;
}

//Da el orden del choque del rayo con dos cajas
// 0 si no choca ninguna caja, 1 si choca solo la primera, 2 si choca solo la segunda
// 3 si choca primero la primera y luego la segunda, 4 si choca primero la segunda y luego la primera
int orden_choque(Ray* rayo, Vector* lim_min_izq, Vector* lim_max_izq, Vector* lim_min_der, Vector* lim_max_der){
    float col_izq = colision_rayo_caja(rayo, lim_min_izq, lim_max_izq);
    float col_der = colision_rayo_caja(rayo, lim_min_der, lim_max_der);
    if (col_izq != -1){
        if (col_der != -1){
            if (col_der < col_izq){
                return 4;
            }
            return 3;
        }
        return 1;
    }
    if (col_der != -1){
        return 2;
    }
    return 0;
}
