#include "AudioCompressorProcessor.h"

namespace Steinberg {
namespace Vst {
	namespace StComp {

		AudioCompressorProcessor::AudioCompressorProcessor() {

		}

		AudioCompressorProcessor::~AudioCompressorProcessor() {

		}

		tresult PLUGIN_API AudioCompressorProcessor::initialize(FUnknown* context) {
			tresult result = AudioEffect::initialize(context);
			if (result == kResultTrue) {
				addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
				addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
			}
			return result;
		}

		tresult PLUGIN_API AudioCompressorProcessor::terminate() {
			return AudioEffect::terminate();
		}

		tresult PLUGIN_API AudioCompressorProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
			if (symbolicSampleSize == kSample32) {
				return kResultTrue;
			}
			return kResultTrue;
		}

		tresult PLUGIN_API AudioCompressorProcessor::setBusArrengements
		(
			SpeakerArrangement* inputs,
			int32 numInputs,
			SpeakerArrangement* outputs,
			int32 numOutputs
		)
		{
			if (numInputs == 1 && numOutputs == 1) {
				if (inputs[0] == SpeakerArr::kStereo && outputs[0] == SpeakerArr::kStereo)
				{
					// stereo channel
					return AudioEffect::setBusArrangements(inputs, numInputs, outputs, numOutputs);
				}
				else if (inputs[0] == SpeakerArr::kMono && outputs[0] == SpeakerArr::kMono)
				{
					// mono channel
					return kResultFalse; // stub
				}
			}
			return kResultFalse;
		}

		tresult PLUGIN_API AudioCompressorProcessor::setState(IBStream* state) {
			return kResultOk;
		}

		tresult PLUGIN_API AudioCompressorProcessor::getState(IBStream* state) {
			return kResultOk;
		}

		tresult AudioCompressorProcessor::process(ProcessData& data) 
		{
			Sample32* inL = data.inputs[0].channelBuffers32[0];
			Sample32* inR = data.inputs[0].channelBuffers32[1];
			Sample32* outL = data.outputs[0].channelBuffers32[0];
			Sample32* outR = data.outputs[0].channelBuffers32[1];

			auto numSamples = data.numSamples;
			for (auto i = 0; i < numSamples; i++) {
				outL[i] = inL[i];
				outR[i] = inR[i];
			}
			return kResultTrue;

		}

	}
}
}
