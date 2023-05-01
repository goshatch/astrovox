#ifndef ENVELOPE_H
#define ENVELOPE_H

#define ADSR_MAX_TIME 5.0f
#define ADSR_MIN_TIME 0.0f
#define ADSR_MAX_LEVEL 1.0f
#define ADSR_MIN_LEVEL 0.0f

struct envelope {
	float attack_time;
	float decay_time;
	float sustain_level;
	float release_time;
	float sample_rate;
	float current_level;
	float time;
	enum { ATTACK, DECAY, SUSTAIN, RELEASE, OFF } state;
};

struct envelope env_init(float attack_time, float decay_time, float sustain_level, float release_time, float sample_rate);
void env_note_on(struct envelope *env);
void env_note_off(struct envelope *env);
float env_process(struct envelope *env);

#endif
