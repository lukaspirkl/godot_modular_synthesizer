#include "modular_synthesizer_editor.h"
#include "modular_synthesizer_graph_editor.h"
#include "modular_synthesizer_player_editor.h"

void ModularSynthesizerEditor::edit(Ref<ModularSynthesizer> p_synth)
{
	graph->edit(p_synth);
	player->edit(p_synth);
}

ModularSynthesizerEditor::ModularSynthesizerEditor()
{
	graph = memnew(ModularSynthesizerGraphEditor);
	graph->set_v_size_flags(SIZE_EXPAND_FILL);
	graph->set_h_size_flags(SIZE_EXPAND_FILL);
	add_child(graph);

	player = memnew(ModularSynthesizerPlayerEditor);
	add_child(player);
}
