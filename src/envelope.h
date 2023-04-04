#ifndef ENVELOPE_H
#define ENVELOPE_H

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

struct envelope init_env(float attack_time, float decay_time, float sustain_level, float release_time, float sample_rate);
void env_note_on(struct envelope *env);
void env_note_off(struct envelope *env);
float env_process(struct envelope *env);

#endif
