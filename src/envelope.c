#include "envelope.h"
#include "constants.h"

struct envelope
env_init(float attack_time, float decay_time, float sustain_level, float release_time)
{
	struct envelope env = {
		.attack_time = attack_time,
		.decay_time = decay_time,
		.sustain_level = sustain_level,
		.release_time = release_time,
		.current_level = 0.0f,
		.time = 0.0f,
		.state = OFF
	};
	return env;
}

void
env_note_on(struct envelope *env)
{
	env->time = 0.0f;
	env->state = ATTACK;
}

void
env_note_off(struct envelope *env)
{
	env->state = RELEASE;
	env->time = 0.0f;
}

float
env_process(struct envelope *env)
{
	float dt = 1.0f / SAMPLE_RATE;

	switch (env->state) {
	case ATTACK:
		env->current_level += dt / env->attack_time;
		if (env->current_level >= 1.0f) {
			env->current_level = 1.0f;
			env->state = DECAY;
		}
		break;
	case DECAY:
		env->current_level -= dt / env->decay_time * (1.0f - env->sustain_level);
		if (env->current_level <= env->sustain_level) {
			env->current_level = env->sustain_level;
			env->state = SUSTAIN;
		}
		break;
	case SUSTAIN:
		// Do nothing, the level remains constant
		break;
	case RELEASE:
		env->current_level -= dt / env->release_time * env->sustain_level;
		if (env->current_level <= 0.0f) {
			env->current_level = 0.0f;
			env->state = OFF;
		}
		break;
	case OFF:
		env->current_level = 0.0f;
		break;
	}

	env->time += dt;
	return env->current_level;
}
