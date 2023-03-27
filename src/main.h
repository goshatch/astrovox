#ifndef MAIN_H
#define MAIN_H

#define SAMPLE_RATE 44100.0
#define BUFFER_SIZE 512

void sigint_handler(int sig);

struct state {
  int gen_index;
  double time_index;
};

#endif
