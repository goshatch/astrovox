#include <math.h>
#include "filter.h"

struct low_pass_filter
init_low_pass_filter(float sample_rate, float cutoff, float resonance)
{
	struct low_pass_filter filter;
	filter.sample_rate = sample_rate;
	filter.cutoff = cutoff;
	filter.resonance = resonance;
	filter.a1 = 0.0f;
	filter.a2 = 0.0f;
	filter.a3 = 0.0f;
	filter.a4 = 0.0f;

	low_pass_filter_update_coefficients(&filter);

	return filter;
}

void
low_pass_filter_set_cutoff(struct low_pass_filter *filter, float cutoff)
{
	if (cutoff < LOPASS_CUTOFF_MIN) {
		filter->cutoff = LOPASS_CUTOFF_MIN;
	} else if (cutoff > LOPASS_CUTOFF_MAX) {
		filter->cutoff = LOPASS_CUTOFF_MAX;
	} else {
		filter->cutoff = cutoff;
	}
	low_pass_filter_update_coefficients(filter);
}

void
low_pass_filter_set_resonance(struct low_pass_filter *filter, float resonance)
{
	if (resonance < LOPASS_RES_MIN) {
		filter->resonance = LOPASS_RES_MIN;
	} else if (resonance > LOPASS_RES_MAX) {
		filter->resonance = LOPASS_RES_MAX;
	} else {
		filter->resonance = resonance;
	}
	low_pass_filter_update_coefficients(filter);
}

void
low_pass_filter_update_coefficients(struct low_pass_filter *filter)
{
	float w = 2 * M_PI * filter->cutoff / filter->sample_rate;
	filter->g = tan(w / 2);
	filter->k = 1 / (filter->resonance + 1);
}

float
low_pass_filter_process(struct low_pass_filter *filter, float input)
{
	float s1 = filter->a1;
	float s2 = filter->a2;
	float s3 = filter->a3;
	float s4 = filter->a4;

	// Update the state-variable filter
	float v_hp = (input - filter->k * s1 - s3) / (1 + filter->g);
	float v_bp = v_hp * filter->g + s1;
	float v_lp = v_bp * filter->g + s3;

	s1 = v_bp;
	s2 = v_hp;
	s3 = v_lp;

	// Cascade the filter
	v_hp = (v_lp - filter->k * s3 - s4) / (1 + filter->g);
	v_bp = v_hp * filter->g + s3;
	v_lp = v_bp * filter->g + s4;

	s3 = v_bp;
	s4 = v_hp;

	filter->a1 = s1;
	filter->a2 = s2;
	filter->a3 = s3;
	filter->a4 = s4;

	return v_lp;
}
