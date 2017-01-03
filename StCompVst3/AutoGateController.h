#pragma once


#include "public.sdk/source/vst/vsteditcontroller.h"
#include "AutoGateConsts.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	class AutoGateController : public EditControllerEx1 {
	public:
		static FUnknown* createInstance(void*) {
			return (IEditController*)new AutoGateController();
		}

		tresult PLUGIN_API initialize(FUnknown* context);
		tresult PLUGIN_API terminate();

		tresult PLUGIN_API setComponentState(IBStream* state);

		IPlugView* PLUGIN_API createView(const char * name);
		tresult PLUGIN_API setParamNormalized(ParamID tag, ParamValue value);
		void editorDestroyed(EditorView* editor) {}
		void editorAttached(EditorView* editor);
		void editorRemoved(EditorView* editor);

		tresult receiveText(const char* text);

		DELEGATE_REFCOUNT(EditControllerEx1)
			tresult PLUGIN_API queryInterface(const char* iid, void** obj);




	};

}
}
}

