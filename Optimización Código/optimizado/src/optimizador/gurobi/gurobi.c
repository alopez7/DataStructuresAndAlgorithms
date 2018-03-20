#include "gurobi.h"
#include <stdio.h>
#include <string.h>

/** Optimiza las cargas de un avion */
void optimize_weight(Route* route)
{
  if (optimizing_weights)
  {
    // Variable donde guardo los outputs de las funciones de gurobi (errores)
    int error = 0;

    // Notacion:
    // Los nodos de la ruta van desde el 0 al n-1
    // Defino que para el nodo con indice i, su q_i es la variable numero 2*i,
    // y su Q_i es la variable numero 2*i + 1

    // Cuento cuantos nodos hay en la ruta y les asigno indices
    int count = 0;
    for (LNode* ln = route -> nodes -> start; ln; ln = ln -> next)
    {
      ln -> route_index = count;
      count++;
    }

    // Arreglo donde guardo la solución
    double* solution = malloc(sizeof(double) * 2 * count);
    // Creo el arreglo con los tipos de las variables
    char* vtype = malloc(sizeof(char) * 2 * count);
    // Creo el arreglo con las constantes que multiplican las variables en la FO
    double* fo_ponderators = malloc(sizeof(double) * 2 * count);
    // Arreglo con los nombres de las variables
    char** var_names = malloc(sizeof(char*) * 2 * count);
    for (int i = 0; i < count; i++)
    {
      // Creo los nombres de las variables
      var_names[2 * i] = calloc(8, sizeof(char));
      sprintf(var_names[2 * i], "q%d", i);
      var_names[2 * i + 1] = calloc(8, sizeof(char));
      sprintf(var_names[2 * i + 1], "Q%d", i);
    }


    ///////////////////////////////////////////////////////////////////////////
    //            Crear el ambiente del modelo de optimizacion               //
    ///////////////////////////////////////////////////////////////////////////

    // Creo la variable de ambiente de gurobi y la completo con la funcion GRBloadenv
    GRBenv *env = NULL;
    // El segundo argumento de la funcion es el archivo donde se guarda el log (puede ser NULL)
    error = GRBloadenv(&env, NULL);
    // Si retorna un error, no sigo
    if (error) goto QUIT;
    error = GRBsetintparam(env, "LogToConsole", 0);
    // Si retorna un error, no sigo
    if (error) goto QUIT;

    ///////////////////////////////////////////////////////////////////////////
    //                        Creo el modelo vacío                           //
    ///////////////////////////////////////////////////////////////////////////

    // Variable que guarda al modelo
    GRBmodel *model = NULL;
    // Le paso a la funcion el ambiente, el modelo, el nombre y otros datos que
    // más adelante voy a inicializar (por ahora tiene 0 variables)
    error = GRBnewmodel(env, &model, "Weights", 0, NULL, NULL, NULL, NULL, NULL);
    // Si da error, termino
    if (error) goto QUIT;

    ///////////////////////////////////////////////////////////////////////////
    //                    Hago que el modelo maximize                        //
    ///////////////////////////////////////////////////////////////////////////

    // Le paso al modelo, la variable a cambiar y el valor nuevo (min a max)
    error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
    // Si da error, termino
    if (error) goto QUIT;


    ///////////////////////////////////////////////////////////////////////////
    //                         Agregar variables                             //
    ///////////////////////////////////////////////////////////////////////////

    // Completo estos arreglos con los tipos de variables y la funcion objetivo
    int i = 0;
    for (LNode* ln = route -> nodes -> start; ln; ln = ln -> next)
    {
      // Hago continua a q_i y Q_i
      vtype[2 * i] = GRB_CONTINUOUS;
      vtype[2 * i + 1] = GRB_CONTINUOUS;
      // Hago que en la FO se calcule q_i * (tarifa_i - costo_dual_i) + 0 * Q_i
      if (ln -> node -> node_type == PICKUP)
      {
        fo_ponderators[2 * i] = ln -> node -> fee - ln -> node -> dual_pi;
      }
      else
      {
        fo_ponderators[2 * i] = 0;
      }
      fo_ponderators[2 * i + 1] = 0;
      i++;
    }

    // Le paso el modelo, la cantidad de variables de la FO y sus tipos
    // Por ahora dejaremos todo en 0 o NULL
    // pasamos fo_const y vtype
    error = GRBaddvars(model, count * 2, 0, NULL, NULL, NULL, fo_ponderators, NULL, NULL, vtype, var_names);
    // Si tenemos un error, terminamos
    if (error) goto QUIT;


    ///////////////////////////////////////////////////////////////////////////
    //                            Restricciones                              //
    ///////////////////////////////////////////////////////////////////////////


    // Argumentos de una restriccion:
    // Le paso el modelo, el numero de variables de la restriccion, los indices y
    // las constantes, el tipo de ecuacion (=, <, >, <=, >=), y la constante del
    // otro lado de la ecuacion
    // al final le doy nombre a la restriccion

    // Los arreglos con nombre r*_ind contienen los indices de las variables
    // Los arreglos con nombre r*_val contienen las constantes que acompañan a las variables

    ////////  R00: q_0 = 0, R01: q_n = 0 (start y end recogen y dejan 0)  ////////

    int r00_ind[1] = { 0 }; //tomo q_0
    double r00_val[1] = { 1.0 }; //les asigno constante 1
    int r01_ind[1] = { 2 * (count -1) }; //tomo q_{n-1}
    double r01_val[1] = { 1.0 }; //les asigno constante 1

    error = GRBaddconstr(model, 1, r00_ind, r00_val, GRB_EQUAL, 0.0, "R00");
    // Si da error, termino
    if (error) goto QUIT;
    error = GRBaddconstr(model, 1, r01_ind, r01_val, GRB_EQUAL, 0.0, "R01");
    // Si da error, termino
    if (error) goto QUIT;

    //////// R1: q_i - q_j = 0, para i, j pickup y delivery ////////

    int r1_ind[2];
    double r1_val[2];
    char r_name[8];
    int r_count = 0;
    // itero sobre la ruta desde el primer pickup hasta el ultimo delivery
    for (LNode* ln = route -> nodes -> start -> next; ln -> next; ln = ln -> next)
    {
      // Si es pickup
      if (ln -> node -> node_type == PICKUP)
      {
        // Obtengo el indice del pickup y del delivery
        int pick_ind = ln -> route_index;
        int deli_ind = ln -> pair -> route_index;
        // Agrego los indices de las variables al arreglo
        r1_ind[0] = pick_ind * 2;
        r1_ind[1] = deli_ind * 2;
        // Agrego los valores de las constantes
        r1_val[0] = 1.0;
        r1_val[1] = -1.0;
        // Creo el nombre de la restriccion
        sprintf(r_name, "R1%d", r_count);
        // Creo la restriccion
        error = GRBaddconstr(model, 2, r1_ind, r1_val, GRB_EQUAL, 0.0, r_name);
        // Si da error, termino
        if (error) goto QUIT;
        // Le sumo 1 al numero de restriccion
        r_count++;
      }
    }

    //////// R2: q_i >= 0, para todos ////////

    int r2_ind[1];
    double r2_val[1];
    r_count = 0;
    // itero sobre la ruta desde el primer pickup hasta el ultimo delivery
    for (LNode* ln = route -> nodes -> start -> next; ln -> next; ln = ln -> next)
    {
      // Obtengo el indice del nodo
      int node_ind = ln -> route_index;
      // Agrego el indice al arreglo
      r2_ind[0] = node_ind * 2;
      // Agrego el valor de la constante
      r2_val[0] = 1.0;
      // Creo el nombre de la restriccion
      sprintf(r_name, "R2%d", r_count);
      // Creo la restriccion
      error = GRBaddconstr(model, 1, r2_ind, r2_val, GRB_GREATER_EQUAL, 0.0, r_name);
      // Si da error, termino
      if (error) goto QUIT;
      // Le sumo 1 al numero de restriccion
      r_count++;
    }

    //////// R3: q_i <= D_i para i pickup ////////

    int r3_ind[1];
    double r3_val[1];
    r_count = 0;
    // itero sobre la ruta desde el primer pickup hasta el ultimo delivery
    for (LNode* ln = route -> nodes -> start -> next; ln -> next; ln = ln -> next)
    {
      // Si es pickup
      if (ln -> node -> node_type == PICKUP)
      {
        // Obtengo el indice del pickup
        int pick_ind = ln -> route_index;
        // Agrego el indice al arreglo
        r3_ind[0] = pick_ind * 2;
        // Agrego el valor de la constante
        r3_val[0] = 1.0;
        // Creo el nombre de la restriccion
        sprintf(r_name, "R3%d", r_count);
        // Creo la restriccion
        error = GRBaddconstr(model, 1, r3_ind, r3_val, GRB_LESS_EQUAL, ln -> node -> total_weight, r_name);
        // Si da error, termino
        if (error) goto QUIT;
        // Le sumo 1 al numero de restriccion
        r_count++;
      }
    }

    //////// R4: Q_i +- q_j = Q_j para (i, j) en la ruta ////////

    // Q_i +- q_j - Q_j = 0

    int r4_ind[3];
    double r4_val[3];
    r_count = 0;
    // itero sobre la ruta desde el primer nodo hasta el pen ultimo nodo
    for (LNode* ln = route -> nodes -> start; ln -> next; ln = ln -> next)
    {
      // Obtengo el indice del nodo actual y del siguiente
      int actual_ind = ln -> route_index;
      int next_ind = ln -> next -> route_index;
      // Agrego los indices a la restriccion
      r4_ind[0] = actual_ind * 2 + 1;
      r4_ind[1] = next_ind * 2;
      r4_ind[2] = next_ind * 2 + 1;
      // Agrego las constantes
      r4_val[0] = 1.0;
      if (ln -> next -> node -> node_type == PICKUP) r4_val[1] = 1.0;
      else r4_val[1] = -1.0;
      r4_val[2] = -1.0;
      // Creo el nombre de la restriccion
      sprintf(r_name, "R4%d", r_count);
      // Creo la restriccion
      error = GRBaddconstr(model, 3, r4_ind, r4_val, GRB_EQUAL, 0, r_name);
      // Si da error, termino
      if (error) goto QUIT;
      // Le sumo 1 al numero de restriccion
      r_count++;
    }

    //////// R5: Q_{n-1} = 0  ////////

    int r5_ind[1] = { 2 * (count - 1) + 1}; //tomo Q_{n-1}
    double r5_val[1] = { 1.0 }; //le asigno constante 1
    error = GRBaddconstr(model, 1, r5_ind, r5_val, GRB_EQUAL, 0.0, "R5");
    // Si da error, termino
    if (error) goto QUIT;

    //////// R6: Q_i <= CAP  ////////

    int r6_ind[1];
    double r6_val[1];
    r_count = 0;
    // itero sobre la ruta desde el primer nodo hasta el ultimo nodo
    for (LNode* ln = route -> nodes -> start; ln; ln = ln -> next)
    {
      // Obtengo el indice del nodo actual
      int actual_ind = ln -> route_index;
      // Agrego el indice a la restriccion
      r6_ind[0] = actual_ind * 2 + 1;
      // Agrego la constante
      r6_val[0] = 1;
      // Creo el nombre de la restriccion
      sprintf(r_name, "R6%d", r_count);
      // Creo la restriccion
      error = GRBaddconstr(model, 1, r6_ind, r6_val, GRB_LESS_EQUAL, route -> airplane -> total_capacity, r_name);
      // Si da error, termino
      if (error) goto QUIT;
      // Le sumo 1 al numero de restriccion
      r_count++;
    }

    //////// R7: Q_i >= 0  ////////

    int r7_ind[1];
    double r7_val[1];
    r_count = 0;
    // itero sobre la ruta desde el primer nodo hasta el ultimo nodo
    for (LNode* ln = route -> nodes -> start; ln; ln = ln -> next)
    {
      // Obtengo el indice del nodo actual
      int actual_ind = ln -> route_index;
      // Agrego el indice a la restriccion
      r7_ind[0] = actual_ind * 2 + 1;
      // Agrego la constante
      r7_val[0] = 1;
      // Creo el nombre de la restriccion
      sprintf(r_name, "R7%d", r_count);
      // Creo la restriccion
      error = GRBaddconstr(model, 1, r7_ind, r7_val, GRB_GREATER_EQUAL, 0, r_name);
      // Si da error, termino
      if (error) goto QUIT;
      // Le sumo 1 al numero de restriccion
      r_count++;
    }

    ///////////////////////////////////////////////////////////////////////////
    //                             Optimizo                                  //
    ///////////////////////////////////////////////////////////////////////////

    // Le paso el modelo y llamo a optimizar
    // Despues de que termine puedo preguntar por los resultados
    error = GRBoptimize(model);
    // Si da error, termino
    if (error) goto QUIT;


    ///////////////////////////////////////////////////////////////////////////
    //                     Escribir modelo en archivo                        //
    ///////////////////////////////////////////////////////////////////////////

    error = GRBwrite(model, "model.lp");
    if (error) goto QUIT;


    ///////////////////////////////////////////////////////////////////////////
    //                 Obtengo resultados del problema                       //
    ///////////////////////////////////////////////////////////////////////////

    // Obtengo el status (optimizado, no solucion, timeout, etc...)
    // guardo el valor en la variable optimstatus
    int optimstatus;
    error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
    // Si hay un error, termino
    if (error) goto QUIT;

    // Obtengo el valor de la funcion objetivo final
    double fo_val;
    error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &fo_val);
    // Si hay error, termino
    if (error) goto QUIT;

    // Obtengo la solucion:
    // le paso un rango de las variables cullos resultados quiero y el arreglo
    // donde los quiero guardar
    error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, count * 2, solution);
    // Si hay error, termino
    if (error) goto QUIT;

    // Si llego hasta aca, es que funciono bien
    if (optimstatus == GRB_OPTIMAL)
    {
      LNode* ln = route -> nodes -> start;
      for (int i = 0; i < count; i++)
      {
        // Asigno los pesos recogidos y dejados
        ln -> delta_weight = solution[2 * i];
        if (ln -> node -> node_type == DELIVERY) ln -> delta_weight *= -1;
        ln -> actual_weight = solution[2 * i + 1];
        ln = ln -> next;
      }
    }
    else if (optimstatus == GRB_INF_OR_UNBD)
    {
      printf("El modelo no tiene solucion o no esta acotado\n");
    }
    else
    {
      printf("Hubo un error\n");
    }

    QUIT:

    // Si hubo un error imprimo su descripcion
    if (error)
    {
      printf("error: %s\n", GRBgeterrormsg(env));
      return;
    }

    // Libero la memoria usada
    GRBfreemodel(model);
    GRBfreeenv(env);
    free(solution);
    free(vtype);
    free(fo_ponderators);
    for (int i = 0; i < 2 * count; i++)
    {
      free(var_names[i]);
    }
    free(var_names);

    // Asigno los pesos recogidos y dejados en la ruta


    // Hago que la ruta sea valida
    route -> valid = true;
  }
  else
  {
    // La ruta se deja como no valida
    route -> valid = false;
  }
}
