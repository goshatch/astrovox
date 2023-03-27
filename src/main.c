#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "main.h"
#include "waveforms.h"

int main(void) {
  signal(SIGINT, sigint_handler);

  wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};
  int gen_count = sizeof generators / sizeof(wave_gen);

  struct state app_state = { .gen_index = 0, .time_index = 0.0 };

  // Initialise ncurses UI
  initscr();
  noecho();
  cbreak();
  nodelay(stdscr, TRUE);

  // NOTE:
  // This is the main application loop. It should do the following:
  // - Run the oscillator -- generate sample for every beat based on selected
  //   waveform. This involves tracking time, so there should be some kind of
  //   clock.
  // - Update the UI to display the waveform from the generated sample
  //   plus all previously generated ones.
  while (1) {
    clear();

    // TODO: Logic goes here
    mvprintw(0, 0, "State: { gen_index: %d, time_index: %f }", app_state.gen_index, app_state.time_index);

    refresh();

    int c = getch();

    if (c == 'q' || c == 'Q') {
      break;
    } else if (c == 'j') {
      app_state.gen_index = (app_state.gen_index - 1 + gen_count) % gen_count;
    } else if (c == 'k') {
      app_state.gen_index = (app_state.gen_index + 1) % gen_count;
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
