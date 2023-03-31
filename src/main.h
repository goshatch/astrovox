#ifndef MAIN_H
#define MAIN_H

#define SAMPLE_RATE 44100.0
#define BUFFER_SIZE 512

#define A4_FREQUENCY 440.0

void sigint_handler(int sig);
double note_frequency(int note_pos);

struct juno_state {
  int gen_index;
  int note;
  double time_index;
};

#endif
