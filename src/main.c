#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "waveforms.h"

int main(void) {
  signal(SIGINT, sigint_handler);

  wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};
  int gen_count = sizeof generators / sizeof(wave_gen);

  struct app_state state = { .gen_index = 0, .time_index = 0.0 };

  // Initialise ncurses UI
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);

  // Create window for displaying waveform
  WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
  box(win, 0, 0);
  wrefresh(win);

  // NOTE:
  // This is the main application loop. It should do the following:
  // - Run the oscillator -- generate sample for every beat based on selected
  //   waveform. This involves tracking time, so there should be some kind of
  //   clock.
  // - Update the UI to display the waveform from the generated sample
  //   plus all previously generated ones.
  while (1) {
    clear();

    // Generate a waveform using the currently selected generator
    double waveform[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++) {
      waveform[i] = generators[state.gen_index](state.time_index);
      state.time_index += 1.0 / SAMPLE_RATE;
    }

    // Display the waveform
    plot_waveform(waveform, BUFFER_SIZE, 1.0, win);

    mvprintw(WINDOW_HEIGHT + 2, 0, "Waveform type: %s (j/k to change) | %f", wave_name(state.gen_index), state.time_index);
    mvprintw(WINDOW_HEIGHT + 3, 0, "Press q to quit");

    refresh();

    int c = getch();

    if (c == 'q' || c == 'Q') {
      break;
    } else if (c == 'j') {
      state.gen_index = (state.gen_index - 1 + gen_count) % gen_count;
    } else if (c == 'k') {
      state.gen_index = (state.gen_index + 1) % gen_count;
    }
  }

  endwin();
  return 0;
}

void sigint_handler(int sig) {
  (void)sig; // Get rid of unused parameter warning
  endwin();
  printf("Exiting.\n");
  exit(0);
}

void plot_waveform(double *waveform, int waveform_len, double max_val, WINDOW *win) {
  // Clear waveform window
  werase(win);
  box(win, 0, 0);

  // Plot waveform
  double y_scale = (double)WINDOW_HEIGHT / (2.0 * max_val);
  double x_scale = (double)WINDOW_WIDTH / waveform_len;
  for (int i = 0; i < waveform_len - 1; i++) {
    int x1 = i * x_scale;
    int x2 = (i + 1) * x_scale;
    int y1 = WINDOW_HEIGHT / 2 - (int)(waveform[i] * y_scale);
    int y2 = WINDOW_HEIGHT / 2 - (int)(waveform[i + 1] * y_scale);
    mvwaddch(win, y1, x1, ACS_DIAMOND); // Plot point on waveform
    if (x1 == x2) {
      continue; // Don't try to draw vertical lines
    }
    double m = ((double)(y2 - y1)) / ((double)(x2 - x1)); // Slope of line
    double b = y1 - m * x1; // Y intercept of line
    int step = (x2 - x1) > 0 ? 1 : -1;
    for (int x = x1; x != x2; x += step) {
      int y = (int)(m * x + b);
      if (y >= 0 && y < WINDOW_HEIGHT) {
        mvwaddch(win, y, x, ACS_DIAMOND); // Plot point on line
      }
    }
  }

  wrefresh(win);
}
