#include <math.h>
#include "waveforms.h"

// TODO: These formulas need to be confirmed!
// Sine wave: y(t) = A * sin(2 * PI * f * t)
double sine_wave_gen(double time) {
  return sin(2.0 * M_PI * time);
}

// Sawtooth wave: y(t) = A * (2 * (f * t - floor(0.5 + f * t)))
double sawtooth_wave_gen(double time) {
  return fmod(time, 1.0) * 2.0 - 1.0;
}

// Square wave: y(t) = A * sign(sin(2 * PI * f * t))
double square_wave_gen(double time) {
  return sin(2.0 * M_PI * time) >= 0 ? 1.0 : -1.0;
}

char* wave_name(enum wave_types type) {
  switch (type) {
    case SINE_WAVE:
      return "Sine";
    case SAWTOOTH_WAVE:
      return "Sawtooth";
    case SQUARE_WAVE:
      return "Square";
    default:
      return "Unknown";
  }
}
