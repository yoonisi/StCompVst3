#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "AudioCompressorConsts.h"

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

		tresult PLUGIN_API setComponentState(IBStream* state);

	};

}
}
}



