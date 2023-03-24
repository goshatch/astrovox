#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "juno.h"

int main(int argc, char **argv) {
  int frequency = 440;
  int duration = 1; // One second
  int num_samples = SAMPLE_RATE * duration;
  sample_generator generators[] = { sine_sample, sawtooth_sample, square_sample };

  int waveform_type = parse_params(argc, argv);
  if (waveform_type == -1)
    return 1;

  double *samples = generate_waveform(frequency, duration, generators[waveform_type]);
  print_waveform(samples, num_samples);

  free(samples);
  return 0;
}

void print_usage(void) {
  printf("Please specify a waveform:\n");
  printf("\t--sine for a sinusoidal waveform\n");
  printf("\t--saw for a sawtooth waveform\n");
  printf("\t--square for a square waveform\n");
}

int parse_params(int argc, char **argv) {
  if (argc < 2) {
    print_usage();
    return -1;
  }

  if (strcmp(argv[1], "--sine") == 0)
    return SINE_WAVE;
  if (strcmp(argv[1], "--saw") == 0)
    return SAWTOOTH_WAVE;
  if (strcmp(argv[1], "--square") == 0)
    return SQUARE_WAVE;

  printf("Error: Invalid argument\n");
  print_usage();
  return -1;
}

double *generate_waveform(int frequency, int duration, sample_generator generator) {
  int num_samples = SAMPLE_RATE * duration;
  double *samples = malloc(num_samples * sizeof(double));

  int i;
  for (i = 0; i < num_samples; i++) {
    double time = (double) i / SAMPLE_RATE;
    samples[i] = generator(time * frequency);
    samples[i] *= 0.9; // Scale the waveform to fit within the range [-1, 1]
  }

  return samples;
}

// Print out an ASCII representation of the waveform
void print_waveform(double *samples, int num_samples) {
  int i, j;

  for (i = 0; i < num_samples; i += 20) {
    printf("%06d |", i);
    for (j = 0; j < (int)(20 * samples[i] + 20.5); j++) {
      printf("*");
    }
    printf("\n");
  }
}

// TODO: These formulas need to be confirmed!
// Sine wave: y(t) = A * sin(2 * PI * f * t)
double sine_sample(double time) {
  return sin(2.0 * PI * time);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
double sawtooth_sample(double time) {
  return fmod(time, 1.0) * 2.0 - 1.0;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
double square_sample(double time) {
  return sin(2.0 * PI * time) >= 0 ? 1.0 : -1.0;
}
