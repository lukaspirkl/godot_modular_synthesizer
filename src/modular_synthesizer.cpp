#include "modular_synthesizer.h"
#include "editor/editor_scale.h"
//#include "editor/editor_node.h"

using namespace Tonic;

Ref<AudioStreamPlayback> MyAudioStream::instance_playback() {

	Ref<MyAudioStreamPlayback> playback;
	playback.instance();
	playback->generator = this;
	return playback;
}
String MyAudioStream::get_stream_name() const {

	return "UserFeed";
}

float MyAudioStream::get_length() const {
	return 0;
}

void MyAudioStream::_bind_methods() {
}

MyAudioStream::MyAudioStream() {
}

////////////////


void MyAudioStreamPlayback::start(float p_from_pos) {
	active = true;
	mixed = 0.0;
	pos = 0;
}

void MyAudioStreamPlayback::stop() {
	active = false;
}
bool MyAudioStreamPlayback::is_playing() const {

	return active; //always playing, can't be stopped
}

int MyAudioStreamPlayback::get_loop_count() const {
	return 0;
}

float MyAudioStreamPlayback::get_playback_position() const {
	return mixed;
}
void MyAudioStreamPlayback::seek(float p_time) {
	//no seek possible
}

void MyAudioStreamPlayback::mix(AudioFrame* p_buffer, float p_rate_scale, int p_frames) {

	synth.fillBufferOfFloats((float *)p_buffer, p_frames, 2);
}

void MyAudioStreamPlayback::_bind_methods() {
}

MyAudioStreamPlayback::MyAudioStreamPlayback() {

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

bool ModularSynthesizerEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("ModularSynthesizer");
}

void ModularSynthesizerEditorPlugin::make_visible(bool p_visible) {
	if (p_visible) {
		button->show();
		editor->make_bottom_panel_item_visible(modular_synthesizer_editor);
		//modular_synthesizer_editor->update_custom_nodes();
		modular_synthesizer_editor->set_process_input(true);
		//modular_synthesizer_editor->set_process(true); //this is commented for shader editor
	} else {
		if (modular_synthesizer_editor->is_visible_in_tree())
			editor->hide_bottom_panel();
		button->hide();
		modular_synthesizer_editor->set_process_input(false);
		//modular_synthesizer_editor->set_process(false); //this is commented for shader editor
	}
}

ModularSynthesizerEditorPlugin::ModularSynthesizerEditorPlugin(EditorNode *p_node) {
	editor = p_node;
	modular_synthesizer_editor = memnew(ModularSynthesizerEditor);
	modular_synthesizer_editor->set_custom_minimum_size(Size2(0, 300) * EDSCALE);

	button = editor->add_bottom_panel_item("Modular Synthesizer" /*TTR("ModularSynthesizer")*/, modular_synthesizer_editor);
	button->hide();
}

ModularSynthesizerEditorPlugin::~ModularSynthesizerEditorPlugin() {
}

ModularSynthesizerEditor::ModularSynthesizerEditor() {
	graph = memnew(GraphEdit);
	graph->get_zoom_hbox()->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_v_size_flags(SIZE_EXPAND_FILL);
	graph->set_h_size_flags(SIZE_EXPAND_FILL);
	graph->set_drag_forwarding(this);
	add_child(graph);

	GraphNode *node = memnew(GraphNode);
	node->set_title("Output");
	node->set_offset(Vector2(100, 100));
	graph->add_child(node);
}
