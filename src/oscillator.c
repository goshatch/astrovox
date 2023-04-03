#include <math.h>
#include "oscillator.h"

wave_gen generators[] = {sine_wave_gen, sawtooth_wave_gen, square_wave_gen};

// TODO: These formulas need to be confirmed!
// Sine wave: y(t) = A * sin(2 * PI * f * t)
float sine_wave_gen(float time) {
  return sin(2.0 * M_PI * time);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
float sawtooth_wave_gen(float time) {
  return fmod(time, 1.0) * 2.0 - 1.0;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
float square_wave_gen(float time) {
  return sin(2.0 * M_PI * time) >= 0 ? 1.0 : -1.0;
}

char* wave_name(enum wave_types type) {
  switch (type) {
    case SINE_WAVE:
      return "◠◡";
    case SAWTOOTH_WAVE:
      return "⩘⩘";
    case SQUARE_WAVE:
      return "⨅_";
    default:
      return "Unknown";
  }
}

struct oscillator init_osc(enum wave_types type, float frequency) {
  struct oscillator osc = {
      .frequency = frequency, .type = type, .generator = generators[type]};
  return osc;
}

void prev_wave_gen(struct oscillator *osc) {
  osc->type = (osc->type - 1 + GEN_COUNT) % GEN_COUNT;
  osc->generator = generators[osc->type];
}

void next_wave_gen(struct oscillator *osc) {
  osc->type = (osc->type + 1) % GEN_COUNT;
  osc->generator = generators[osc->type];
}
