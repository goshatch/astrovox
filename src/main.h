#ifndef MAIN_H
#define MAIN_H

#define SAMPLE_RATE 44100.0
#define BUFFER_SIZE 80

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 20

/* Forward declaration for the ncurses WINDOW type */
struct WINDOW;

void sigint_handler(int sig);
void plot_waveform(double *waveform, int waveform_len, double max_val, WINDOW *win);

struct app_state {
  int gen_index;
  double time_index;
};

#endif
