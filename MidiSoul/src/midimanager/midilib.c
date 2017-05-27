#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <smf.h>

#include "midilib.h"

#define PATH_TEMP "src/midimanager/temp.mid"



void midi_read_from_file(const char* filename){
  smf_t* smf;
  smf_track_t* smf_track;
  smf_event_t* smf_event;

  smf = smf_load(filename);
  if (smf == NULL) {
    fprintf(stderr,"Errror en lectura de MIDI %s\n", filename);
    abort();
  }
  int track_count = smf->number_of_tracks;
  int max_cant = 0;
  int total_events = 0;
  for(int t = 1; t < track_count; t++){
    smf_track = smf_get_track_by_number(smf, t+1);
    int event_count = smf_track -> number_of_events;
    total_events += event_count;
    if (event_count > max_cant) max_cant = event_count;
  }


  // Guardado info relevante
  int events[track_count-1][max_cant+1][5];
  smf_event_t** events_t;
  events_t = malloc(total_events * sizeof(smf_event_t));

  //Por cada track revisar eventos, partimos del segundo ya que el primero es metadata
  total_events = 0;
  for(int t = 0; t < track_count; t++){
    smf_track = smf_get_track_by_number(smf, t+1);

    //Revisar cada evento
    int real_count = 0;
    while((smf_event = smf_track_get_next_event(smf_track)) != NULL) {

      //Casos con información no relevante
      if(smf_event_is_eot(smf_event)){
        events_t[total_events] = smf_event;
        total_events++;
        continue;
      }
      if(t == 0) continue;
      if(!smf_event_is_valid(smf_event) || smf_event_is_metadata(smf_event) ||
        smf_event_is_sysex(smf_event) || smf_event_is_system_common(smf_event) ||
        smf_event_is_system_realtime(smf_event)){
          continue;
      }

      int type = -1;
      int note = smf_event -> midi_buffer[1];
      int event_id = smf_event -> midi_buffer[0] & 0xF0;
      int chann = smf_event -> midi_buffer[0];
      if(event_id == 144){
        type = 1;
        if(!(smf_event -> midi_buffer[2])){
          type = 0;
        }
      }
      else if(event_id == 128){
        chann += 16;
        type = 0;
      }

      if(type == -1) continue;

      //Caso de error, primer evento es off
      if(type == 0 && real_count == 0){
        continue;
      }

      events[t-1][real_count+1][0] = type;
      events[t-1][real_count+1][1] = note;
      events[t-1][real_count+1][2] = smf_event -> time_pulses;
      events[t-1][real_count+1][3] = chann;
      events[t-1][real_count+1][4] = smf_event -> midi_buffer[2];

      real_count++;

      events_t[total_events] = smf_event;
      total_events++;

    }
    if(t > 0)  events[t-1][0][0] = real_count;
  }

  // Vaciar el midi
  for(int e = 0; e < total_events; e++){
    smf_event_remove_from_track(events_t[e]);
  }
  for(int e = 0; e < total_events; e++){
    smf_event_delete(events_t[e]);
  }
  free(events_t);



  //Crear temp.mid con metadata y sin notas
  int ret = smf_save(smf, PATH_TEMP);
  if(ret){
    fprintf(stderr, "Error al guardar temp.midi\n");
    abort();
  }
  smf_delete(smf);


  //Impresión
  fprintf(stdout,"%d\n", track_count-1);
  for(int t = 0; t < track_count-1; t++){
    int n_events = events[t][0][0];
    fprintf(stdout,"%d\n", n_events);
    for(int e = 1; e < n_events+1; e++){
      int type = events[t][e][0];
      int note = events[t][e][1];
      int e_time = events[t][e][2];
      int chann = events[t][e][3];
      int vel = events[t][e][4];
      fprintf(stdout,"%d %d %d %d %d\n", type, note, e_time, chann, vel);
    }
  }
}

void midi_write_to_file (const char* filename){
  smf_t* smf1;
  smf_t* smf2 = smf_new();
  smf_event_t* smf1_event;
  smf_event_t* smf2_event;
  smf_track_t* smf1_track;
  smf_track_t* smf2_track;
  smf1 = smf_load(PATH_TEMP);
  if (smf1 == NULL) {
    fprintf(stderr,"Errror en lectura de temp.midi\n");
    abort();
  }

  int format = smf_set_format(smf2, 0);
  int ppqn = smf_set_ppqn(smf2, smf1 -> ppqn);
  if (format && ppqn){
    fprintf(stderr,"Error en el parseo de metadata desde temp.midi\n");
    abort();
  }

  int track_count, event_count, type, note, e_time, chann, vel;
  fscanf(stdin, "%d\n", &track_count);


  int all_tracks = track_count+1;
  for(int t = 0; t < all_tracks; t++){
    smf1_track = smf_get_track_by_number(smf1, t+1);
    smf2_track = smf_track_new();
    smf_add_track(smf2, smf2_track);
    while((smf1_event = smf_track_get_next_event(smf1_track)) != NULL) {
      smf2_event = smf_event_new_from_pointer(smf1_event -> midi_buffer, smf1_event -> midi_buffer_length);
      smf_track_add_event_pulses(smf2_track, smf2_event, 0);
    }
    smf_event_delete(smf2_event);

    fscanf(stdin, "%d\n", &event_count);
    for(int e = 0; e < event_count; e++){
      fscanf(stdin, "%d %d %d %d %d\n", &type, &note, &e_time, &chann, &vel);
      if(type){
        smf2_event = smf_event_new_from_bytes(chann, note, vel);
      }
      else{
        smf2_event = smf_event_new_from_bytes(chann - 16, note, 0);
      }
      smf_track_add_event_pulses(smf2_track, smf2_event, e_time + 1);
    }
    int aux = smf_track_add_eot_pulses(smf2_track, e_time + 1);
    if(aux) fprintf(stderr, "Error al agregar eot %s\n", filename);
  }

  int ret = smf_save(smf2, filename);
  if(ret){
    fprintf(stderr, "Error al guardar %s\n", filename);
    abort();
  }

  smf_event_delete(smf2_event);
  smf_track_delete(smf1_track);
  smf_track_delete(smf2_track);
  smf_delete(smf1);
  smf_delete(smf2);

  remove(PATH_TEMP);

}
