#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "midilib.h"

int main(int argc, char const *argv[])
{
	/* El programa recibe 2 parametros */
	if(argc != 3)
	{
		fprintf(stderr, "Modo de uso: %s <mode> <file.mid>\n", argv[0]);
		fprintf(stderr, "\t<mode> es el sentido en el que se debe trabajar \n");
		fprintf(stderr, "\t<file.mid> el input o output del programa \n");
		return 1;
	}

	if(!strcmp(argv[1],"miditoevents"))
	{
		// fprintf(stderr, "Modo MIDI to Music\n");

		// fprintf(stderr, "Leeré %s e imprimiré todos sus eventos de notas\n", argv[2]);

		midi_read_from_file(argv[2]);

	}
	else if(!strcmp(argv[1],"eventstomidi"))
	{
		// fprintf(stderr, "Modo Music to MIDI\n");

		// fprintf(stderr, "Leeré de la consola eventos de notas y escribiré %s\n", argv[2]);

		midi_write_to_file(argv[2]);

	}
	else
	{
		fprintf(stderr, "INVALID MODE: %s\n", argv[2]);

		return 10;
	}

	return 0;
}
