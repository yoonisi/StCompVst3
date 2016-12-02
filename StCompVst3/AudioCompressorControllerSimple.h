#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "AudioCompressorConsts.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	class AudioCompressorControllerSimple : public EditController
	{
	public:
		tresult PLUGIN_API initialize(FUnknown* context);
		static FUnknown* createInstance(void*) {
			return (IEditController*)new AudioCompressorControllerSimple();
		}
		tresult PLUGIN_API setComponentState(IBStream* state);
	};

}
}
}



