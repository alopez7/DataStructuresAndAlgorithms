# Modo de uso

Compilar el programa por consola con el comando:
```
make
```

Ejecutar en consola:

```
./optimizador [orders.txt] [airplanes.txt] [PB.txt] (-ow)
```

Donde:

* [orders.txt] es el archivo con los pedidos
* [airplanes.txt] es el archivo con las aviones
* [PB.txt] es el archivo con la planificación base
* (-ow) es opcional y determina si se optimizan los pesos en una ruta o si se toman solo los pesos completos

Ejemplo ejecutando la instacia de 35 pedidos optimizando los pesos:
```
./optimizador files/AA35/orders.txt files/AA35/airplanes.txt files/AA35/PB.txt -ow
```

# Formato de los archivos

## Pedidos:

La primera linea indica cuantos nodos hay (el doble de los pedidos).
Las siguientes lineas corresponden cada un nodo, y se indican los siguientes elementos:

```
[ID] [tipo] [pos_x] [pos_y] [duracion] [peso] [hora_inicio] [hora_fin] [tarifa]
```

* El tipo puede ser "p" o "d", lo que indica que es pickup o delivery.
* pos_x y pos_y indican la posicion del nodo en el plano.
* La duración es el tiempo que se demora en hacer el pickup o el delivery.
* El peso es el total de la carga, y es negativo en caso de ser un nodo de delivery.
* hora_inicio y hora_fin corresponden al rango de tiempo en los cuales esta disponible el nodo para el pickup o delivery.
* La tarifa es la ganancia por hacer el pickup (es o en caso de ser delivery)
* Si hay "n" pedidos, el pickup de ID "i" corresponde al delivery de ID = i + n

## Aviones:

La primera linea indica cuantos aviones (n) hay y el costo de penalización por cancelación.

Las siguientes 2*n lineas dan la informacion de los nodos de inicio y de fin:

```
[avion] [tipo] [pos_x] [pos_y] [hora_inicio] [hora_fin]
```

* El avion es un ID del avion correspondiente.
* El tipo puede ser "s" o "e" (start o end).
* pos_x y pos_y corresponden a la posicion del nodo en el plano.
* hora_inicio y hora_fin corresponden a la ventana de tiempo en los que se pueden visitar los nodos.

Las últimas n lineas dan la capacidad de cada avion (peso).

## Planificacion base:

La primera linea indica cuantas rutas hay (es el mismo número que la cantidad de aviones).

Las siguientes lineas contienen las rutas asignadas a cada avión. El primer número indica la cantidad de nodos de la ruta, y el resto son los índices de los nodos.
