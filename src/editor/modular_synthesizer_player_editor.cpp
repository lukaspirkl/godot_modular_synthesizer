#include "scene/gui/panel.h"
#include "scene/gui/label.h"
#include "scene/gui/button.h"
#include "scene/gui/slider.h"
#include "scene/audio/audio_stream_player.h"

#include "../../thirdparty/tonic/src/Tonic.h"

#include "modular_synthesizer_player_editor.h"
#include "../modular_synthesizer_playback.h"

void ModularSynthesizerPlayerEditor::_button_pressed()
{
	if (audio_stream_player->is_playing())
	{
		audio_stream_player->stop();
		play_stop->set_text("Play");
	}
	else
	{
		audio_stream_player->play();
		play_stop->set_text("Stop");
	}
}

void ModularSynthesizerPlayerEditor::_res_changed()
{
	playback->_refresh_synth();
	_refresh_realtime_parameters();
}

void ModularSynthesizerPlayerEditor::_refresh_realtime_parameters()
{
	for (List<Node*>::Element* E = property_nodes.back(); E; E = E->prev()) {
		Node* n = E->get();
		remove_child(n);
		memdelete(n);
	}
	property_nodes.clear();

	for (auto& param : playback->synth->getParameters())
	{
		String name = param.getName().c_str();

		if (param.getParameterType() == Tonic::ControlParameterType::ControlParameterTypeToggle)
		{
			Button* b = memnew(Button);
			b->set_text(name);
			b->connect("button_down", this, "_toggled", varray(true, name));
			b->connect("button_up", this, "_toggled", varray(false, name));
			add_child(b);
			property_nodes.push_back(b);
		}
		else
		{
			Label* l = memnew(Label);
			l->set_text(name);
			add_child(l);
			property_nodes.push_back(l);

			HSlider* s = memnew(HSlider);
			s->set_step(0.01);
			s->set_min(param.getMin());
			s->set_max(param.getMax());
			s->set_value(param.getValue());
			s->connect("value_changed", this, "_value_changed", varray(name));
			add_child(s);
			property_nodes.push_back(s);
		}
	}
}

void ModularSynthesizerPlayerEditor::_toggled(bool p_value, const String& p_name)
{
	playback->set_parameter(p_name, p_value ? 1.0f : 0.0f);
}

void ModularSynthesizerPlayerEditor::_value_changed(float p_value, const String& p_name)
{
	playback->set_parameter(p_name, p_value);
}

void ModularSynthesizerPlayerEditor::_bind_methods()
{
	ClassDB::bind_method("_button_pressed", &ModularSynthesizerPlayerEditor::_button_pressed);
	ClassDB::bind_method("_res_changed", &ModularSynthesizerPlayerEditor::_res_changed);
	ClassDB::bind_method("_toggled", &ModularSynthesizerPlayerEditor::_toggled);
	ClassDB::bind_method("_value_changed", &ModularSynthesizerPlayerEditor::_value_changed);
}

void ModularSynthesizerPlayerEditor::edit(Ref<ModularSynthesizer> p_synth)
{
	p_synth->connect("changed", this, "_res_changed");
	audio_stream_player->set_stream(p_synth);
	playback = Object::cast_to<ModularSynthesizerPlayback>(audio_stream_player->get_stream_playback().ptr());
	_refresh_realtime_parameters();
}

ModularSynthesizerPlayerEditor::ModularSynthesizerPlayerEditor()
{
	audio_stream_player = memnew(AudioStreamPlayer);
	add_child(audio_stream_player);

	play_stop = memnew(Button);
	play_stop->set_size(Size2(200, 10));
	play_stop->set_text("Play");
	play_stop->connect("pressed", this, "_button_pressed");
	add_child(play_stop);
}
