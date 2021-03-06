#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"
#include "vst3ids.h"
#include "EnvelopeGenerator.h"
#include <memory>

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
			tresult PLUGIN_API setupProcessing(ProcessSetup& newSetup);


			static FUnknown* createInstrance(void*) {
				return (IAudioProcessor*)(new AudioCompressorProcessor);
			}

		private:
			SampleRate sampleRate;
			std::unique_ptr<EnvelopeGenerator<double> > envelopeGenerator;
			std::unique_ptr<ParamValue[]> parameters;
			bool processParameterChanges(IParameterChanges* changes);
			bool processEvenets(IEventList* events);
			void setParameter(int index, ParamValue paramValue, int32 sampleOffset);
			double makeUpGain;

			template<typename T>
			void audioProcessing(ProcessData& data,int samples, T* inL, T* inR, T* outL, T* outR);

			template<typename T>
			void audioProcessingMono(ProcessData& data, int samples, T* in, T*out);

		};

}
}
}


