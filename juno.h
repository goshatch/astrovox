#ifndef JUNO_H
#define JUNO_H

#include <ncurses.h>

#define SAMPLE_RATE 44100.0
#define BUFFER_SIZE 512

#define ROWS 20
#define COLS 80

enum wave_types { SINE_WAVE, SAWTOOTH_WAVE, SQUARE_WAVE };

typedef double (*sample_generator)(double);

// double *generate_waveform(int frequency, int duration, sample_generator generator);
void generate_waveform(sample_generator generator, double time_offset,
                       double time_step, int rows, int cols, WINDOW *win);
double sine_sample(double time);
double sawtooth_sample(double time);
double square_sample(double time);

void print_waveform(double *samples, int num_samples);
void print_usage(void);
int parse_params(int argc, char **argv);

void sigint_handler(int sig);
#endif
