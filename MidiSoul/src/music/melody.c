#include "melody.h"
#include <string.h>
#include <stdbool.h>

/** Traduce la información del MIDI a una melodía de elementos secuenciales */
Melody* melody_from_midi (MIDI* midi)
{
	/* Tomamos solo el primer track del midi */
	Track* track = midi -> track_array[0];

	/* Obtenemos la descripción de los eventos del midi */

	int event_count = track -> event_count;

	int element_count = 0;
	int cache[event_count][2];

	for (int e = 0; e < event_count - 1; e++)
	{
			Event e1 = track -> event_array[e];
			Event e2 = track -> event_array[e + 1];
			/* Diferencia de tiempo entre este evento y el siguiente */
			int dif = e2.time - e1.time;
			cache[e][0] = dif;
			cache[e][1] = e1.note;
			if(dif > 0)
			{
				element_count++;
			}
	}

	/* Armamos la melodía */
	Melody* melody = malloc(sizeof(Melody));
	melody -> element_array = malloc(event_count * sizeof(Element));
	melody -> length = element_count;

	int element_id = 0;

	for (int e = 0; e < event_count - 1; e++)
	{
		if(cache[e][0])
		{
			int* md = malloc(2 * sizeof(int));
			memcpy(md, track -> event_array[e].metadata, 2 * sizeof(int));
			melody -> element_array[element_id].metadata = md;
			melody -> element_array[element_id].length = cache[e][0];

			//silencio
			if(e % 2)
			{
				uint8_t prev = melody -> element_array[element_id - 1].note;
				melody -> element_array[element_id].note = prev;
				melody -> element_array[element_id].type = REST;
			}
			//nota
			else
			{
				melody -> element_array[element_id].note = cache[e][1];
				melody -> element_array[element_id].type = NOTE;
			}
			element_id++;
		}
	}

	return melody;

}

/** Traduce una melodía de vuelta a MIDI */
MIDI*   melody_to_midi   (Melody* melody)
{
	MIDI* midi = malloc(sizeof(MIDI));
	int element_count = melody -> length;
	int event_count = 0;

	for (int i = 0; i < element_count; i++)
	{
		/* Cada nota es en realidad dos eventos, uno de inicio y uno de termino */
		if(melody -> element_array[i].type == NOTE)
		{
			event_count += 2;
		}
	}

	midi -> track_count = 1;
	midi -> track_array = malloc(sizeof(Track));

	Track* track = malloc(sizeof(Track));
	track -> event_count = event_count;
	track -> event_array = malloc(event_count * sizeof(Event));

	int event_id = 0;
	int actual_time = 0;
	for (int i = 0; i < element_count; i++)
	{
		Element element = melody -> element_array[i];
		if(element.type == NOTE)
		{
			int* md1 = malloc(2 * sizeof(int));
			int* md2 = malloc(2 * sizeof(int));
			memcpy(md1, element.metadata, 2 * sizeof(int));
			memcpy(md2, element.metadata, 2 * sizeof(int));

			// Inicio de la nota
			track -> event_array[event_id].metadata = md1;
			track -> event_array[event_id].note = element.note;
			track -> event_array[event_id].start = true;
			track -> event_array[event_id].time = actual_time;

			// Final de la nota
			track -> event_array[event_id + 1].metadata = md2;
			track -> event_array[event_id + 1].note = element.note;
			track -> event_array[event_id + 1].start = false;
			track -> event_array[event_id + 1].time = actual_time + element.length;
			event_id += 2;
		}
		actual_time += element.length;
	}
	midi -> track_array[0] = track;

	return midi;
}

/** Libera los recursos asociados a esta melodía */
void    melody_destroy   (Melody* melody)
{
	for(int i = 0 ; i < melody -> length; i++)
	{
		free(melody -> element_array[i].metadata);
	}
	free(melody -> element_array);
	free(melody);
}
