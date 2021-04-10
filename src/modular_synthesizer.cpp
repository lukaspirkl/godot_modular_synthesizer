#include "modular_synthesizer.h"

using namespace Tonic;

Ref<AudioStreamPlayback> ModularSynthesizer::instance_playback() {

	Ref<ModularSynthesizerPlayback> playback;
	playback.instance();
	playback->generator = this;
	return playback;
}
String ModularSynthesizer::get_stream_name() const {

	return "UserFeed";
}

float ModularSynthesizer::get_length() const {
	return 0;
}

void ModularSynthesizer::_bind_methods() {
}

ModularSynthesizer::ModularSynthesizer() {
}

////////////////

void ModularSynthesizerPlayback::start(float p_from_pos) {
	active = true;
	mixed = 0.0;
	pos = 0;
}

void ModularSynthesizerPlayback::stop() {
	active = false;
}
bool ModularSynthesizerPlayback::is_playing() const {

	return active; //always playing, can't be stopped
}

int ModularSynthesizerPlayback::get_loop_count() const {
	return 0;
}

float ModularSynthesizerPlayback::get_playback_position() const {
	return mixed;
}
void ModularSynthesizerPlayback::seek(float p_time) {
	//no seek possible
}

void ModularSynthesizerPlayback::mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) {

	synth.fillBufferOfFloats((float *)p_buffer, p_frames, 2);
}

void ModularSynthesizerPlayback::_bind_methods() {
}

ModularSynthesizerPlayback::ModularSynthesizerPlayback() {

	generator = NULL;
	active = false;
	mixed = 0;
	pos = 0;

	Tonic::setSampleRate(44100);

	ControlMetro metro = ControlMetro().bpm(100);
	ControlGenerator freq = ControlRandom().trigger(metro).min(0).max(1);

	Generator tone = SquareWaveBL().freq(
							 freq * 0.25 + 100 + 400) *
					 SineWave().freq(50);

	ADSR env = ADSR()
					   .attack(0.01)
					   .decay(0.4)
					   .sustain(0)
					   .release(0)
					   .doesSustain(false)
					   .trigger(metro);

	StereoDelay delay = StereoDelay(3.0f, 3.0f)
								.delayTimeLeft(0.5 + SineWave().freq(0.2) * 0.01)
								.delayTimeRight(0.55 + SineWave().freq(0.23) * 0.01)
								.feedback(0.3)
								.dryLevel(0.8)
								.wetLevel(0.2);

	Generator filterFreq = (SineWave().freq(0.01) + 1) * 200 + 225;

	LPF24 filter = LPF24().Q(2).cutoff(filterFreq);

	Generator output = ((tone * env) >> filter >> delay) * 0.3;

	synth.setOutputGen(output);
}
