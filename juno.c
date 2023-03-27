#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include "juno.h"

// NOTE: Old code to be removed.

int main(int argc, char **argv) {
  signal(SIGINT, sigint_handler);

  int frequency = 440;
  int waveform_index = 0;

  sample_generator generators[] = { sine_sample, sawtooth_sample, square_sample };

  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);

  // NOTE:
  // This function should really be the main application loop. It should do the
  // following:
  // - Run the oscillator -- generate sample for every beat based on selected
  //   waveform. This involves tracking time, so there should be some kind of
  //   clock.
  // - Update the UI to display the waveform from the generated sample
  //   plus all previously generated ones.
  while (1) {
    float buffer[BUFFER_SIZE];

    for (int i = 0; i < BUFFER_SIZE; i++) {
      // TODO:
      // This will no longer return a double* because we're no longer getting a
      // waveform of a specific duration, rather we want the "position" of the
      // wave at a specific time index. The duration is however long the loop
      // runs for.
      buffer[i] = generate_waveform(waveform_index, i * 1.0 / SAMPLE_RATE);
    }

    clear();

    for (int i = 0; i < BUFFER_SIZE; i += 2) {
      int row = (int)(BUFFER_SIZE / 2 - buffer[i] * BUFFER_SIZE / 2);
      mvprintw(row, i / 2, "*");
    }

    refresh();

    int c = getch();

    if (c == 'q' || c == 'Q') {
      break;
    } else if (c == KEY_LEFT) {
      waveform_index = (waveform_index - 1 + num_generators) % num_generators;
    } else if (c == KEY_RIGHT) {
      waveform_index = (waveform_index + 1) % num_generators;
    }
  }

  /* free(samples); */
  return 0;
}

void sigint_handler(int sig) {
  (void)sig; // Get rid of unused parameter warning
  endwin();
  printf("Exiting.\n");
  exit(0);
}

/* void print_usage(void) { */
/*   printf("Please specify a waveform:\n"); */
/*   printf("\t--sine for a sinusoidal waveform\n"); */
/*   printf("\t--saw for a sawtooth waveform\n"); */
/*   printf("\t--square for a square waveform\n"); */
/* } */

/* int parse_params(int argc, char **argv) { */
/*   if (argc < 2) { */
/*     print_usage(); */
/*     return -1; */
/*   } */

/*   if (strcmp(argv[1], "--sine") == 0) */
/*     return SINE_WAVE; */
/*   if (strcmp(argv[1], "--saw") == 0) */
/*     return SAWTOOTH_WAVE; */
/*   if (strcmp(argv[1], "--square") == 0) */
/*     return SQUARE_WAVE; */

/*   printf("Error: Invalid argument\n"); */
/*   print_usage(); */
/*   return -1; */
/* } */

/* double *generate_waveform(int frequency, int duration, sample_generator generator) { */
/*   int num_samples = SAMPLE_RATE * duration; */
/*   double *samples = malloc(num_samples * sizeof(double)); */

/*   int i; */
/*   for (i = 0; i < num_samples; i++) { */
/*     double time = (double) i / SAMPLE_RATE; */
/*     samples[i] = generator(time * frequency); */
/*     samples[i] *= 0.9; // Scale the waveform to fit within the range [-1, 1] */
/*   } */

/*   return samples; */
/* } */

/* void generate_waveform(sample_generator generator, double time_offset, */
/*                        double time_step, int rows, int cols, WINDOW *win) { */
/*   int row, col; */
/*   double time = time_offset; */
/*   for (row = 0; row < rows; row++) { */
/*     wmove(win, row, 0); */
/*     for (col = 0; col < cols; col++) { */
/*       double sample = generator(time); */
/*       int ch = (int)(sample * (cols / 2) + (cols / 2)); */
/*       waddch(win, ch >= cols ? ACS_HLINE : ch <= 0 ? ' ' : ch); */
/*       time += time_step; */
/*     } */
/*   } */
/* } */

void generate_waveform(sample_generator generator, double time_offset,
                       double time_step, int rows, int cols, WINDOW *win) {
  int row, col;
  double time = time_offset;
  for (row = 0; row < rows; row++) {
    wmove(win, row, 0);
    for (col = 0; col < cols; col++) {
      double sample = generator(time);
      int ch = (int)(sample * (cols / 2) + (cols / 2));
      waddch(win, ch >= cols ? ACS_HLINE : ch <= 0 ? ' ' : ch);
      time += time_step;
    }
  }
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
  return sin(2.0 * M_PI * time);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
double sawtooth_sample(double time) {
  return fmod(time, 1.0) * 2.0 - 1.0;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
double square_sample(double time) {
  return sin(2.0 * M_PI * time) >= 0 ? 1.0 : -1.0;
}
