#ifndef WAVEFORMS_H
#define WAVEFORMS_H

enum wave_types { SINE_WAVE, SAWTOOTH_WAVE, SQUARE_WAVE };

typedef double (*wave_gen)(double);

double sine_wave_gen(double time);
double sawtooth_wave_gen(double time);
double square_wave_gen(double time);

#endif
