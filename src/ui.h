#ifndef UI_H
#define UI_H

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 21

#define MAX_FPS 24

#define WAVEFORM_ZOOM_FACTOR 10.0

/* Forward declaration for the ncurses WINDOW type */
struct WINDOW;
struct state;

struct ui {
	WINDOW *win;
	float frame_duration;
};

struct ui init_ui(void);
void ui_tick(struct state *state, int waveform_len, float max_val);
void plot_waveform(float *waveform, int waveform_len, float max_val, WINDOW *win);
void get_note_name(int key_position, char *note_name);
void print_osc_status_line(struct state *state);
void print_env_status_line(struct state *state);
void print_filter_status_line(struct state *state);
void print_midi_status_line(struct state *state);
#endif
