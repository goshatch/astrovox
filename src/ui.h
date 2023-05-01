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
	WINDOW *waveform_win;
	WINDOW *status_win;
	float frame_duration;
};

struct ui ui_init(void);
void ui_tick(struct state *state, int waveform_len, float max_val);
void ui_plot_waveform(float *waveform, int waveform_len, float max_val, WINDOW *win);
void ui_get_note_name(int key_position, char *note_name);
void ui_print_osc_status_line(struct state *state);
void ui_print_env_status_line(struct state *state);
void ui_print_filter_status_line(struct state *state);
void ui_print_midi_status_line(struct state *state);
#endif
