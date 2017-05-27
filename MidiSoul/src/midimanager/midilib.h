#ifndef T2_LIB_MIDI
#define T2_LIB_MIDI

/** Lee un archivo .midi y lo imprime en consola para que el padre lo lea  */
void midi_read_from_file (const char* filename);
/** Lee la consola para escribir un archivo .midi*/
void midi_write_to_file (const char* filename);

#endif /* end of include guard: T2_LIB_MIDI */
