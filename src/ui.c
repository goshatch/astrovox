#include <locale.h>
#include <ncurses.h>
#include <stdlib.h>

#include "main.h"
#include "state.h"
#include "ui.h"

struct ui
init_ui(void)
{
	const float frame_duration = 1.0 / MAX_FPS;

	// Initialise ncurses UI
	setlocale(LC_ALL, "");
	initscr();
	noecho();
	cbreak();
	nodelay(stdscr, TRUE);

	if (has_colors()) {
		start_color();
		init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Teal color pair
	}

	WINDOW *waveform_win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
	WINDOW *status_win = newwin(4, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	wrefresh(waveform_win);
	box(waveform_win, 0, 0); // Draw the box around the waveform window

	struct ui ui = {
		.waveform_win = waveform_win,
		.status_win = status_win,
		.frame_duration = frame_duration
	};
	return ui;
}

void
ui_tick(struct state *state, int waveform_len, float max_val)
{
	werase(state->ui.waveform_win); // Clear the waveform window
	plot_waveform(state->vis_waveform, waveform_len, max_val, state->ui.waveform_win);
	box(state->ui.waveform_win, 0, 0); // Draw the box around the waveform window
	wnoutrefresh(state->ui.waveform_win);

	print_osc_status_line(state);
	print_filter_status_line(state);
	print_env_status_line(state);
	print_midi_status_line(state);
	wnoutrefresh(state->ui.status_win);

	doupdate();
}

void
plot_waveform(float *waveform, int waveform_len, float max_val, WINDOW *win)
{
	// Clear waveform window
	werase(win);

	// Padding at the top and bottom of the waveform
	int v_padding = 1;

	// Plot waveform
	float y_scale = (float)(WINDOW_HEIGHT - (4 * v_padding)) / (2.0 * max_val);
	float x_scale = (float)WINDOW_WIDTH / (waveform_len / WAVEFORM_ZOOM_FACTOR);

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
}

void
get_note_name(int key_position, char *note_name)
{
	const char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

	if (key_position == -1) {
		snprintf(note_name, 4, "-");
	} else {
		int midi_note = key_position + 12; // Calculate the MIDI note number
		int octave = midi_note / 12 - 2;   // Find the octave number
		int note_index =
			midi_note % 12; // Calculate the note index within the octave
		snprintf(note_name, 4, "%s%d", note_names[note_index], octave);
	}
}

void
print_midi_status_line(struct state *state)
{
	int row = 3;
	wattron(state->ui.status_win, A_BOLD);
	mvwprintw(state->ui.status_win, row, 0, "\t| MIDI");
	wattroff(state->ui.status_win, A_BOLD);

	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, " %d: %s %s", state->input.midi_device_id, state->input.selected_midi_device_info->interf, state->input.selected_midi_device_info->name);
	wattroff(state->ui.status_win, COLOR_PAIR(1));
}

void
print_osc_status_line(struct state *state)
{
	int row = 0;
	char note_name[4];
	get_note_name(state->voices[0].note.value, note_name);

	wattron(state->ui.status_win, A_BOLD);
	mvwprintw(state->ui.status_win, row, 0, "VOICE1\t| OSC");
	wattroff(state->ui.status_win, A_BOLD);

	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, " %s", wave_name(state->voices[0].osc.type));
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " | ");

	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%s", note_name);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " | ");

	wattron(state->ui.status_win, A_BOLD);
	wprintw(state->ui.status_win, "TIME ");
	wattroff(state->ui.status_win, A_BOLD);

	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%f", state->wave_time_index);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " | ");

	wattron(state->ui.status_win, A_BOLD);
	wprintw(state->ui.status_win, "CHORUS ");
	wattroff(state->ui.status_win, A_BOLD);

	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%s", state->voices[0].chorus ? wave_name(state->voices[0].chorus_osc.type) : "OFF  ");
	wattroff(state->ui.status_win, COLOR_PAIR(1));
}

void
print_env_status_line(struct state *state)
{
	int row = 2;
	wattron(state->ui.status_win, A_BOLD);
	mvwprintw(state->ui.status_win, row, 0, "\t| ENV");
	wattroff(state->ui.status_win, A_BOLD);

	wprintw(state->ui.status_win, " A");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].env.attack_time);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " D");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].env.decay_time);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " S");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].env.sustain_level);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " R");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].env.release_time);
	wattroff(state->ui.status_win, COLOR_PAIR(1));

	wprintw(state->ui.status_win, " | state: %d, level: %.3f",
		state->voices[0].env.state,
		state->voices[0].env.current_level);
}

void
print_filter_status_line(struct state *state)
{
	int row = 1;
	wattron(state->ui.status_win, A_BOLD);
	mvwprintw(state->ui.status_win, row, 0, "\t| LOPASSF");
	wattroff(state->ui.status_win, A_BOLD);

	wprintw(state->ui.status_win, " CUTOFF");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].filter.cutoff);
	wattroff(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "Hz");

	wprintw(state->ui.status_win, " RES Q");
	wattron(state->ui.status_win, COLOR_PAIR(1));
	wprintw(state->ui.status_win, "%.2f", state->voices[0].filter.resonance);
	wattroff(state->ui.status_win, COLOR_PAIR(1));
}
