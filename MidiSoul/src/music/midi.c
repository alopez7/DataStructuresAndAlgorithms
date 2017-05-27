#include "midi.h"
#include <stdio.h>
#include <stdlib.h>

#define MIDIMANAGER "./midimanager"

/** Construye la representación lógica del MIDI a partir de un archivo */
MIDI* midi_from_file (char* filename)
{
	/* Invocamos al midi manager para que lea el midi por nosotros */
	char command[256];
  sprintf(command, "%s %s %s", MIDIMANAGER, "miditoevents" ,filename);

	/* child = translator.stdout */
  FILE* child = popen(command, "r");

	MIDI* midi;
	Track* track;
	int track_count, event_count, type, note, e_time, chann, vel;

	fscanf(child, "%d\n", &track_count);
	midi = malloc(sizeof(MIDI));
	midi -> track_count = track_count;
	midi -> track_array = malloc(track_count * sizeof(Track));

	for (int t = 0; t < track_count; t++)
	{
		fscanf(child, "%d\n", &event_count);
		track = malloc(sizeof(Track));
		track -> event_array = malloc(event_count*sizeof(Event));
		track -> event_count = event_count;


		for(int e = 0; e < event_count; e++)
		{
			fscanf(child, "%d %d %d %d %d\n", &type, &note, &e_time, &chann, &vel);
			track -> event_array[e].note = note;
			track -> event_array[e].time = e_time;
			track -> event_array[e].start = type;

			//metadata de evento
			int* md = malloc(2 * sizeof(int));
			md[0] = chann;
			md[1] = vel;
			track -> event_array[e].metadata = md;
		}
		midi -> track_array[t] = track;
	}
	pclose(child);

	return midi;
}

/** Guarda la representación logica de un MIDI en un archivo */
void  midi_to_file   (MIDI* midi, char* filename)
{
	/* Invocamos al midi manager para que guarde el midi por nosotros */
	char command[256];
	sprintf(command, "%s %s %s", MIDIMANAGER, "eventstomidi" ,filename);

	/* child = translator.stdin */
	FILE* child = popen(command, "w");

	Track* track;
	int track_count = midi -> track_count;

  //Impresión número de tracks
  fprintf(child, "%d\n", track_count);
	for(int t = 0; t < track_count; t++)
	{
		track = midi -> track_array[t];
		int event_count = track -> event_count;

    //Impresión número de eventos en track actual
		fprintf(child, "%d\n", event_count);
		for(int e = 0; e < event_count; e++)
		{
			Event event = track -> event_array[e];
			int type = 0;
			if(event.start) type = 1;

      //Impresión de toda la información de evento en tuplas de enteros
			int* md = event.metadata;
			fprintf(child, "%d %d %d %d %d\n", type, event.note, (int)event.time, md[0], md[1]);
    }
	}

	/* Hay que hacer flush para que efectivamente se mande */
	fflush(child);
	pclose(child);
}

/** Libera los recursos asociados a este archivo midi */
void  midi_destroy   (MIDI* midi)
{
	for(int t = 0; t < midi -> track_count ; t++)
	{
		for (int e = 0; e < midi -> track_array[t] -> event_count ; e++)
		{
			free(midi -> track_array[t] -> event_array[e].metadata);
		}
		free(midi -> track_array[t] -> event_array);
		free(midi -> track_array[t]);
	}
	free(midi -> track_array);
	free(midi);
}
