#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>

#include "main.h"
#include "ui.h"
#include "waveforms.h"

struct juno_ui init_ui(void) {
  const double frequency = 20.0; // 20 Hz
  const double increment =
      frequency / SAMPLE_RATE * (BUFFER_SIZE / (double)(WINDOW_WIDTH - 2));
  const double frame_duration = 1.0 / MAX_FPS;

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

  struct juno_ui ui = {.win = win,
                       .frequency = frequency,
                       .time_index_increment = increment,
                       .frame_duration = frame_duration};
  return ui;
}

void ui_tick(struct juno_ui *ui, struct juno_state *state, double *waveform,
             int waveform_len, double max_val) {
  werase(ui->win); // Clear the waveform window
  plot_waveform(waveform, waveform_len, max_val, ui->win);
  box(ui->win, 0, 0); // Draw the box around the waveform window

  // Refresh the waveform window without updating the screen
  wnoutrefresh(ui->win);
  mvprintw(WINDOW_HEIGHT, 0, "OSC %s | TIME %f", wave_name(state->gen_index),
           state->time_index);
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
    int y1 =
        (WINDOW_HEIGHT - 4) / 2 - (int)(waveform[i] * y_scale) + v_padding + 1;
    int y2 = (WINDOW_HEIGHT - 4) / 2 - (int)(waveform[i + 1] * y_scale) +
             v_padding + 1;

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
