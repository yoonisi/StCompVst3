#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"
#include "vst3ids.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

		class AudioCompressorProcessor : public AudioEffect
		{
		public:
			AudioCompressorProcessor();
			virtual ~AudioCompressorProcessor();
			tresult PLUGIN_API initialize(FUnknown* contexxt);
			tresult PLUGIN_API terminate();
			tresult PLUGIN_API canProcessSampleSize(int32 symbokicSampleSize);
			tresult PLUGIN_API setBusArrengements(
				SpeakerArrangement* inputs,
				int32 numInputs,
				SpeakerArrangement* outputs,
				int32 numOutputs
			);
			tresult PLUGIN_API setState(IBStream* state);
			tresult PLUGIN_API getState(IBStream* state);
			tresult PLUGIN_API process(ProcessData& data);

			static FUnknown* createInstrance(void*) {
				return (IAudioProcessor*)(new AudioCompressorProcessor);
			}
		};

}
}
}


