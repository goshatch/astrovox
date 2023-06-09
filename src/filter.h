#ifndef FILTER_H
#define FILTER_H

#define LOPASS_CUTOFF_MIN 20.0
#define LOPASS_CUTOFF_MAX 2000.0

#define LOPASS_RES_MIN 0.0
#define LOPASS_RES_MAX 2.0

struct low_pass_filter {
	float cutoff;
	float resonance;
	float g;
	float k;
	float a1;
	float a2;
	float a3;
	float a4;
};

struct low_pass_filter low_pass_filter_init(float cutoff, float resonance);
void low_pass_filter_set_cutoff(struct low_pass_filter *filter, float cutoff);
void low_pass_filter_set_resonance(struct low_pass_filter *filter, float resonance);
void low_pass_filter_update_coefficients(struct low_pass_filter *filter);
float low_pass_filter_process(struct low_pass_filter *filter, float input);

#endif
