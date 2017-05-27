#ifndef T2_LIB_MELODY
#define T2_LIB_MELODY

/* ######################################################################## */
/*                                Melodías                                  */
/*                                                                          */
/* Este módulo se encarga de la representación lógica de una melodía como   */
/* un arreglo secuencial de notas y silencios. Se proveen funciones para    */
/* convertir entre melodía y MIDI                                           */
/* ######################################################################## */

#include <stdlib.h>
#include <stdint.h>
#include "midi.h"

/* ######################################################################## */
/*                                 TIPOS                                    */
/* ######################################################################## */

/** Representa un símbolo de notación musical, ya sea nota o silencio */
enum musical_symbol
{
	/** Representa una nota musical */
	NOTE,
	/** Representa un silencio */
	REST
};

/** Representa un símbolo de notación musical, ya sea nota o silencio */
typedef enum musical_symbol Symbol;

/** Representa un elemento musical con un símbolo, duración y nota */
struct musical_element
{
	/** Información importante pero no relevante para el problema */
	void* metadata;

	/** Indica el tipo de elemento musical con el símbolo que corresponde */
	Symbol type;
	/** La duración de este elemento */
	uint32_t length;
	/** El número de la nota, en caso de ser nota */
	uint8_t note;
};

/** Representa un elemento musical con un símbolo, duración y nota */
typedef struct musical_element Element;

/** Representa una melodía; un conjunto de notas y silencios en orden */
struct melody
{
	/** Largo de la melodía: cantidad de símbolos que la componen */
	size_t length;
	/** Arreglo de elementos musicales */
	Element* element_array;
};

/** Representa una melodía; un conjunto de notas y silencios en orden */
typedef struct melody Melody;

/* ######################################################################## */
/*                               FUNCIONES                                  */
/* ######################################################################## */

/** Traduce la información del MIDI a una melodía de elementos secuenciales */
Melody* melody_from_midi (MIDI* midi);
/** Traduce una melodía de vuelta a MIDI */
MIDI*   melody_to_midi   (Melody* melody);
/** Libera los recursos asociados a esta melodía */
void    melody_destroy   (Melody* melody);

#endif /* end of include guard: T2_LIB_MELODY */
