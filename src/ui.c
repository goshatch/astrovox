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

	// Create window for displaying waveform
	WINDOW *win = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
	wrefresh(win);
	box(win, 0, 0); // Draw the box around the waveform window

	struct ui ui = {.win = win,
		.frame_duration = frame_duration};
	return ui;
}

void
ui_tick(struct ui *ui, struct state *state, int waveform_len, float max_val)
{
	werase(ui->win); // Clear the waveform window
	plot_waveform(state->vis_waveform, waveform_len, max_val, ui->win);
	box(ui->win, 0, 0); // Draw the box around the waveform window

	// Refresh the waveform window without updating the screen
	wnoutrefresh(ui->win);
	print_osc_status_line(state);
	print_filter_status_line(state);
	print_env_status_line(state);
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

	wnoutrefresh(win);
	doupdate();
}

const char *note_names[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

void
get_note_name(int key_position, char *note_name)
{
	if (key_position == -1) {
		snprintf(note_name, 4, "-");
	} else {
		int midi_note = key_position + 20; // Calculate the MIDI note number
		int octave = midi_note / 12 - 1;   // Find the octave number
		int note_index =
			midi_note % 12; // Calculate the note index within the octave
		snprintf(note_name, 4, "%s%d", note_names[note_index], octave);
	}
}

void
print_osc_status_line(struct state *state)
{
	char note_name[4];
	get_note_name(state->voices[0].note.value, note_name);

	attron(A_BOLD);
	mvprintw(WINDOW_HEIGHT, 0, "VOICE1\t| OSC");
	attroff(A_BOLD);

	attron(COLOR_PAIR(1));
	printw(" %s", wave_name(state->voices[0].osc.type));
	attroff(COLOR_PAIR(1));

	printw(" | ");

	attron(COLOR_PAIR(1));
	printw("%s", note_name);
	attroff(COLOR_PAIR(1));

	printw(" | ");

	attron(A_BOLD);
	printw("TIME ");
	attroff(A_BOLD);

	attron(COLOR_PAIR(1));
	printw("%f", state->time_index);
	attroff(COLOR_PAIR(1));
}

void
print_env_status_line(struct state *state)
{
	attron(A_BOLD);
	mvprintw(WINDOW_HEIGHT + 2, 0, "\t| ENV");
	attroff(A_BOLD);

	printw(" A");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].env.attack_time);
	attroff(COLOR_PAIR(1));

	printw(" D");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].env.decay_time);
	attroff(COLOR_PAIR(1));

	printw(" S");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].env.sustain_level);
	attroff(COLOR_PAIR(1));

	printw(" R");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].env.release_time);
	attroff(COLOR_PAIR(1));

	printw(" | state: %d, level: %.3f",
		state->voices[0].env.state,
		state->voices[0].env.current_level);
}

void
print_filter_status_line(struct state *state)
{
	attron(A_BOLD);
	mvprintw(WINDOW_HEIGHT + 1, 0, "\t| LOPASSF");
	attroff(A_BOLD);

	printw(" CUTOFF");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].filter.cutoff);
	attroff(COLOR_PAIR(1));
	printw("Hz");

	printw(" RES Q");
	attron(COLOR_PAIR(1));
	printw("%.2f", state->voices[0].filter.resonance);
	attroff(COLOR_PAIR(1));
}
