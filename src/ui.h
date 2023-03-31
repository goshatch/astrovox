#ifndef UI_H
#define UI_H

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 21

#define MAX_FPS 24

/* Forward declaration for the ncurses WINDOW type */
struct WINDOW;
struct juno_state;

struct juno_ui {
  WINDOW *win;
  double frequency;
  double time_index_increment;
  double frame_duration;
};

struct juno_ui init_ui(void);
void ui_tick(struct juno_ui *ui, struct juno_state *state, double *waveform, int waveform_len, double max_val);
void plot_waveform(double *waveform, int waveform_len, double max_val, WINDOW *win);

#endif
