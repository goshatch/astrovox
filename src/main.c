#include <locale.h>
#include <math.h>
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "waveforms.h"

int main(void) {
  signal(SIGINT, sigint_handler);

  wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};
  int gen_count = sizeof generators / sizeof(wave_gen);

  struct app_state state = { .gen_index = 0, .time_index = 0.0 };

  double frequency = 20.0; // 20 Hz
  double increment =
      frequency / SAMPLE_RATE * (BUFFER_SIZE / (double)(WINDOW_WIDTH - 2));

  const double frame_duration = 1.0 / MAX_FPS;
  double last_frame_time = 0.0;

  // Initialise ncurses UI
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);

  // Create window for displaying waveform
  WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
  wrefresh(win);
  box(win, 0, 0); // Draw the box around the waveform window

  while (1) {
    // Generate a waveform using the currently selected generator
    double waveform[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; i++) {
      waveform[i] = generators[state.gen_index](state.time_index);
      state.time_index += increment;
    }

    // Find the maximum absolute value of the waveform
    double max_val = 0.0;
    for (int i = 0; i < BUFFER_SIZE; i++) {
      double abs_val = fabs(waveform[i]);
      if (abs_val > max_val) {
        max_val = abs_val;
      }
    }

    // Display the waveform if it's time to redraw
    double current_time = (double)clock() / CLOCKS_PER_SEC;
    if (current_time - last_frame_time < frame_duration) { continue; }
    last_frame_time = current_time;

    werase(win); // Clear the waveform window
    plot_waveform(waveform, BUFFER_SIZE, max_val, win);
    box(win, 0, 0); // Draw the box around the waveform window
    wnoutrefresh(win); // Refresh the waveform window without updating the screen
    mvprintw(WINDOW_HEIGHT, 0, "OSC %s | TIME %f",
             wave_name(state.gen_index), state.time_index);
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

  // Padding at the top and bottom of the waveform
  int v_padding = 1;

  // Plot waveform
  double y_scale = (double)(WINDOW_HEIGHT - (4 * v_padding)) / (2.0 * max_val);
  double x_scale = (double)WINDOW_WIDTH / waveform_len;

  // Bresenham line-drawing algorithm
  for (int i = 0; i < waveform_len - 1; i++) {
    int x1 = i * x_scale + 1;
    int x2 = (i + 1) * x_scale + 1;
    int y1 = (WINDOW_HEIGHT - 4) / 2 - (int)(waveform[i] * y_scale) + v_padding + 1;
    int y2 = (WINDOW_HEIGHT - 4) / 2 - (int)(waveform[i + 1] * y_scale) + v_padding + 1;

    // Draw a line between the two points
    int y_diff = abs(y2 - y1);
    int x_diff = abs(x2 - x1);
    int y_sign = y1 < y2 ? 1 : -1;
    int y = y1;

    if (x_diff >= y_diff) {
      int err = x_diff / 2;
      for (int x = x1; x <= x2; x++) {
        mvwaddch(win, y, x, '*');
        err -= y_diff;
        if (err < 0) {
          y += y_sign;
          err += x_diff;
        }
      }
    } else {
      int err = y_diff / 2;
      for (int y = y1; y != y2 + y_sign; y += y_sign) {
        mvwaddch(win, y, x1, '*');
        err -= x_diff;
        if (err < 0) {
          x1++;
          err += y_diff;
        }
      }
    }
  }

  wnoutrefresh(win);
  doupdate();
}
