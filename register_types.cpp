/* register_types.cpp */

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"
#include "src/modular_synthesizer_editor_plugin.h"
#endif

#include "register_types.h"
#include "core/class_db.h"
#include "src/modular_synthesizer.h"

#include "thirdparty/tonic/src/Tonic.h"

void register_modular_synthesizer_types() {
	Tonic::setSampleRate(44100);

	ClassDB::register_class<NodeData>();
	ClassDB::register_class<ConnectionData>();
	ClassDB::register_class<ModularSynthesizer>();
	ClassDB::register_class<ModularSynthesizerPlayback>();
#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<ModularSynthesizerEditorPlugin>();
#endif
}

void unregister_modular_synthesizer_types() {
   // Nothing to do here in this example.
}
