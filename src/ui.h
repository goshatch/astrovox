#ifndef UI_H
#define UI_H

#define WINDOW_WIDTH 80
#define WINDOW_HEIGHT 21

#define MAX_FPS 24

#define WAVEFORM_ZOOM_FACTOR 10.0

/* Forward declaration for the ncurses WINDOW type */
struct WINDOW;
struct juno_state;

struct juno_ui {
	WINDOW *win;
	float frame_duration;
};

struct juno_ui init_ui(void);
void ui_tick(struct juno_ui *ui, struct juno_state *state, int waveform_len, float max_val);
void plot_waveform(float *waveform, int waveform_len, float max_val, WINDOW *win);
void get_note_name(int key_position, char *note_name);

#endif
