#ifndef JUNO_H
#define JUNO_H

#define SAMPLE_RATE 44100.0

#define SINE_WAVE 0
#define SAWTOOTH_WAVE 1
#define SQUARE_WAVE 2

typedef double (*sample_generator)(double);

double *generate_waveform(int frequency, int duration, sample_generator generator);
double sine_sample(double time);
double sawtooth_sample(double time);
double square_sample(double time);

void print_waveform(double *samples, int num_samples);
void print_usage(void);
int parse_params(int argc, char **argv);

#endif
