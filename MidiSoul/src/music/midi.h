#ifndef T2_WRAP_MIDI
#define T2_WRAP_MIDI

/* ######################################################################## */
/*                                  MIDI                                    */
/*                                                                          */
/* Contiene la representación logica y las funciones asociadas a un archivo */
/* de audio en formato MIDI.                                                */
/* ######################################################################## */

#include <stdint.h>
#include <stdbool.h>

/* ######################################################################## */
/*                                 TIPOS                                    */
/* ######################################################################## */

/** Representa un evento de nota. Pertenece a una pista específica */
struct event
{
	/** Información importante pero no relevante para el problema */
	void* metadata;

	/** Indica el momento en el que sucede este evento */
	uint64_t time;
	/** El número correspondiente a la nota */
	uint8_t note;
	/** Indica si el evento es de inicio o final de la nota */
	bool start;
};
/** Representa un evento en el tiempo. Pertenece a un track específico */
typedef struct event Event;

/** Representa una pista de audio, compuesta de eventos en el tiempo */
struct track
{
	/** Los eventos de la pista, ordenados por tiempo */
	Event* event_array;
	/** Cantidad de eventos de nota de la pista */
	int event_count;
};
/** Representa una pista de audio, compuesta de eventos en el tiempo */
typedef struct track Track;

/** Representa un archivo MIDI, compuesto de una serie de pistas de audio */
struct midi
{
	/** Las pistas que componen este archivo */
	Track** track_array;
	/** La cantidad de pistas que lo componen */
	int track_count;
};
/** Representa un archivo MIDI, compuesto de una serie de pistas de audio */
typedef struct midi MIDI;

/* ######################################################################## */
/*                               FUNCIONES                                  */
/* ######################################################################## */

/** Construye la representación lógica del MIDI a partir de un archivo */
MIDI* midi_from_file (char* filename);
/** Guarda la representación logica de un MIDI a un archivo */
void  midi_to_file   (MIDI* midi, char* filename);
/** Libera los recursos asociados a este archivo midi */
void  midi_destroy   (MIDI* midi);



#endif /* end of include guard: T2_WRAP_MIDI */
