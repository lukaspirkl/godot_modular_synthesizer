/* register_types.cpp */

#ifdef TOOLS_ENABLED
#include "editor/editor_plugin.h"
#include "src/modular_synthesizer_editor_plugin.h"
#endif

#include "register_types.h"
#include "core/class_db.h"
#include "src/modular_synthesizer.h"


void register_modular_synthesizer_types() {
	ClassDB::register_class<ModularSynthesizer>();
	ClassDB::register_class<ModularSynthesizerPlayback>();
#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<ModularSynthesizerEditorPlugin>();
#endif
}

void unregister_modular_synthesizer_types() {
   // Nothing to do here in this example.
}
