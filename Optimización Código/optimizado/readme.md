# Como ejecutar el programa

## Pasos previos

Para poder hacer funcionar gurobi con el programa hay que crear una carpeta llamada "lib" en la carpeta "optimizado" que contenga los archivos compilados de gurobi para el sistema operativo correspondiente: libgurobi.so.7.5.2 y libgurobi75.so (la version puede variar). 

## Compilar

Para compilar hay que estar en la carpeta "optimizado" con la consola y escribir

```
make
```

Esto generará un archivo compilado llamado "optimizador" y una carpeta obj con archivos compilados

Para limpiar todos los archivos binarios y compilados hay que escribir en consola

```
make clean
```

Esto borrará la carpeta obj y el archivo "optimizador"

## Ejecutar el programa

Para ejecutar el programa este debe estar compilado

Escribir en consola 

```
./optimizado [orders_file] [airplanes_file] [BP_file] (-ow)  
```

Donde:

"optimizado" es el compilado del programa

orders_file es el archivo que contiene los pedidos

airplanes_file es el archivo que tiene las aviones

BP_file es el archivo que tiene la planificación base

-ow es un parametro opcional que determina si se optimizan los pesos o no. Si se incluye se optimizaran los pesos con gurobi, sino se descartarán las rutas que no puedan cargar todo el peso de los pedidos

### Ejemplo:

Para ejecutar el programa con la instancia de 30 pedidos optimizando los pesos

```
./optimizador files/AA30/airplanes.txt files/AA30/orders.txt files/AA30/BP.txt -ow 
```

# Formato de los archivos

## Orders:

La primera linea indica cuantos nodos de pedidos hay (el doble de los pedidos).
Las siguientes lineas corresponden cada un nodo, y se indican los siguientes elementos:

```
[ID] [tipo] [pos_x] [pos_y] [duracion] [peso] [hora_inicio] [hora_fin] [tarifa]
```

Los IDs deben ir desde el 0 hasta 2 * pedidos - 1

El tipo puede ser "p" o "d", lo que indica que es pickup o delivery.

pos_x y pos_y indican la posicion del nodo en el plano.

La duración es el tiempo que se demora en hacer el pickup o el delivery.

El peso es el total de la carga, y es negativo en caso de ser un nodo de delivery.

hora_inicio y hora_fin corresponden al rango de tiempo en los cuales esta disponible el nodo para el pickup o delivery.

La tarifa es la ganancia por hacer el pickup (es o en caso de ser delivery)

Si hay "n" pedidos, el pickup de ID "i" corresponde al delivery de ID = i + n

## Aviones:

La primera linea indica cuantos aviones (n) hay y el costo de penalización por cancelación.

Las siguientes 2 * n lineas dan la informacion de los nodos de inicio y de fin:

```
[avion] [tipo] [pos_x] [pos_y] [hora_inicio] [hora_fin]
```

El avion es un ID del avion correspondiente.

El tipo puede ser "s" o "e" (start o end).

pos_x y pos_y corresponden a la posicion del nodo en el plano.

hora_inicio y hora_fin corresponden a la ventana de tiempo en los que se pueden visitar los nodos.

Las últimas n lineas dan la capacidad de cada avion (peso).

Los IDs deben partir en 2 * pedidos y terminar en 2 * pedidos + 2 * aviones - 1

## Planificacion base:

La primera linea indica cuantas rutas hay (es el mismo número que la cantidad de aviones).

Las siguientes lineas contienen las rutas asignadas a cada avión. El primer número indica la cantidad de nodos de la ruta, y el resto son los índices de los nodos.

# Output del programa

Al ejecutar el programa se generarán dos archivos con resultados: results.txt y details.txt

results.txt contiene los resultados de cada corrida dentro de la iteración: Utilidad, rutas en la solución, probabilidades de las operaciones y cantidad de veces que se ejecutó cada iteración.

details.txt contiene los detalles de la solución final: Utilidad, tiempo total, rutas finales con asignaciones de tiempos y utilidad de cada una.

El orden de las operaciones es: 

```
Drop and Add
Swap
IRRR
IRRE
IRMRR
IRMRE
Delete
```

## Cálculo de probabilidades

Para calcular las probabilidades de usó la siguiente técnica:

Cada operación parte con 1 punto. Si al utilizar la operación esta mejora, se multiplica sus puntos por 2. Si no mejora, se resta 1 a sus puntos. Cada operación puede tener mínimo 1 punto y máximo 50.

La probabilidad de ejecutar una operación es su cantidad de puntos dividido en los puntos totales
