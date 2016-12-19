#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "AudioCompressorConsts.h"
#include "AudioCompressorEditor.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	class AudioCompressorControllerSimple : public EditControllerEx1
	{
	public:
		static FUnknown* createInstance(void*) {
			return (IEditController*)new AudioCompressorControllerSimple();
		}

		tresult PLUGIN_API initialize(FUnknown* context);
		tresult PLUGIN_API terminate();

		// edit controller
		tresult PLUGIN_API setComponentState(IBStream* state);

		IPlugView* PLUGIN_API createView(const char * name);
		tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value);
		void editorDestroyed(EditorView* editor){}
		void editorAttached(EditorView* editor);
		void editorRemoved(EditorView* editor);

		tresult receiveText(const char* text);

		DELEGATE_REFCOUNT(EditController)
		tresult PLUGIN_API queryInterface(const char* iid, void** obj);

		void addDependentView(AudioCompressorEditor* view);
		void removeDependentView(AudioCompressorEditor* view);

	private:
		TArray<AudioCompressorEditor*> viewArray;

	};

}
}
}



