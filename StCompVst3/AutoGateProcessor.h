#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"
#include <memory>
#include "EnvelopeGenerator.h"
#include "IirHpf.h"
#include "IirLpf.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	class AutoGateProcessor : public AudioEffect
	{
	public:
		AutoGateProcessor();
		virtual ~AutoGateProcessor();
		
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
		tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup);
		static FUnknown* createInstrance(void*) {
			return (IAudioProcessor*)(new AutoGateProcessor);
		}

	private:
		SampleRate sampleRate;
		std::unique_ptr<IirHpf<double> > highPassFilter;
		std::unique_ptr<IirLpf<double> > lowPassFilter;
		std::unique_ptr<ParamValue[]> parameters;

		bool processParameterChanges(IParameterChanges* changes);
		bool processEvenets(IEventList* events);
		void setParameter(int index, ParamValue paramValue, int32 sampleOffset);


	};

}
}
}