#include "synth_audio_stream.h"

using namespace Tonic;

Ref<AudioStreamPlayback> SynthAudioStream::instance_playback() {

	Ref<SynthAudioStreamPlayback> playback;
	playback.instance();
	playback->generator = this;
	return playback;
}
String SynthAudioStream::get_stream_name() const {

	return "UserFeed";
}

float SynthAudioStream::get_length() const {
	return 0;
}

void SynthAudioStream::_bind_methods() {
}

SynthAudioStream::SynthAudioStream() {
}

////////////////

void SynthAudioStreamPlayback::start(float p_from_pos) {
	active = true;
	mixed = 0.0;
	pos = 0;
}

void SynthAudioStreamPlayback::stop() {
	active = false;
}
bool SynthAudioStreamPlayback::is_playing() const {

	return active; //always playing, can't be stopped
}

int SynthAudioStreamPlayback::get_loop_count() const {
	return 0;
}

float SynthAudioStreamPlayback::get_playback_position() const {
	return mixed;
}
void SynthAudioStreamPlayback::seek(float p_time) {
	//no seek possible
}

void SynthAudioStreamPlayback::mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames) {

	synth.fillBufferOfFloats((float *)p_buffer, p_frames, 2);
}

void SynthAudioStreamPlayback::_bind_methods() {
}

SynthAudioStreamPlayback::SynthAudioStreamPlayback() {

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
