#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../music/melody.h"
#include "suffixArray.h"

/** Revisa que los parámetros sean válidos */
void check_parameters(int argc, char *argv[])
{
	/* Si se recibieron 4 parámetros */
	if(argc == 5)
	{
		/* Se revisa si es el flag del bonus */
		if(!strcmp(argv[4], "-b"))
		{
			/* Si es, entonces termina el programa sin errores */
			exit(0);
			// TODO si implementas el bonus, entonces tu programa tiene que seguir
		}
		/* Sino, es un parámetro inválido. */
		/* El if de abajo se encargará de terminar el programa */
	}

	/* El programa recibe 3 parametros. Si tenemos algo distinto entonces... */
	if(argc != 4)
	{
		printf("Modo de uso: %s <input.mid> <mu> <output.mid>\n", argv[0]);
		printf("\t<input.midi> es el archivo midi con el que trabajará \n");
		printf("\t<mu> es la cantidad mínima de notas de la melodía repetida \n");
		printf("\t<output.midi> es donde se guardará el audio \n");
		/* Fin del programa, codigo de error 1 */
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	/* Revisa que los parámetros sean correctos antes de empezar */
	check_parameters(argc, argv);

	/* Lee el midi */
	MIDI* midi = midi_from_file(argv[1]);

	/* Obtiene la melodía a partir del midi*/
	Melody* melody = melody_from_midi(midi);

	/* Destruye el midi, ya no se necesita*/
	midi_destroy(midi);

	//MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO

	// Transformo la melody para que se menaje por diferencia de notas en vez de la nota misma
	melody_diff(melody);

	// Creo el suffix array y lo ordeno
	SuffixArray* sufArray = suffixArray_init(melody);

	// Creo el arreglo que me dice cuantos elementos en comun tiene un sufijo de otro
	int* com = suffixArray_common_elements(sufArray);

	//Transformo la melody para que tenga notas en vez de diferencias
	melody_undo_diff(melody);

	//Guardo el largo original para restituirlo despues
	int original_length = melody -> length;

	//Encuentro la secuencia mas repetida y la guardo en la melody, y retorno el puntero al arreglo completo
	Element* mem = find_frase(melody, sufArray, com, atoi(argv[2]));

	//IMPRIMO LA SOLUCION
	for (int i = 0; i < melody -> length; i++)
	{
		element_print(melody -> element_array[i]);
	}
	printf("\n");

	//MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO


	/* Obtiene el midi a partir de la melodía resultante */
	midi = melody_to_midi(melody);

	/* Guarda el midi como archivo */
	midi_to_file(midi, argv[3]);

	/* Destruye el midi */
	midi_destroy(midi);

	//MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO

	//Remplazo mem y length en la melody para poder liberarlo
	melody -> element_array = mem;
	melody -> length = original_length;

	// Libero el suffix array
	suffixArray_destroy(sufArray);
	// Libero la memoria del arreglo com
	free(com);

	//MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO MI CODIGO

	/* Destruye la melodía */
 	melody_destroy(melody);

	return 0;
}
