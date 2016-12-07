#include "AudioCompressorProcessor.h"
#include "AudioCompressorConsts.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/base/ibstream.h"

#include "logger.h"

namespace Steinberg {
namespace Vst {
	namespace StComp {

		using namespace LogTool;

		AudioCompressorProcessor::AudioCompressorProcessor() : 
			sampleRate(44.1e3), 
			makeUpGain(1.0),
			envelopeGenerator(new EnvelopeGenerator<double>),
			parameters(new ParamValue[ParameterIds::kNumParams])
		{
			setControllerClass(AudioCompressorControllerSimpleID);
			for (int i = 0; i < ParameterIds::kNumParams; i++) {
				switch (i)
				{
				case ParameterIds::kThreshold:
					setParameter(i, 1.0, 0);
					break;
				case ParameterIds::kAttack:
					setParameter(i, 0.1, 0);
					break;
				case ParameterIds::kRelease:
					setParameter(i, 0.5, 0);
					break;
				default:
					setParameter(i, 0, 0);
					break;
				}
			}
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

		tresult PLUGIN_API AudioCompressorProcessor::setupProcessing(ProcessSetup& newSetup) {
			sampleRate = newSetup.sampleRate;
			return AudioEffect::setupProcessing(newSetup);
		}

		bool AudioCompressorProcessor::processParameterChanges(IParameterChanges * changes)
		{
			if (!changes) { return false; }
			int32 count = changes->getParameterCount();
			if (count == 0) { return false; }
			for (int32 i = 0; i < count; i++) {
				IParamValueQueue* queue = changes->getParameterData(i);
				if (!queue) { continue; }
				auto paramId = queue->getParameterId();
				auto pointCount = queue->getPointCount();
				int32 sampleOffset(0);
				ParamValue value(0);
				queue->getPoint(pointCount - 1, sampleOffset, value);
				this->setParameter(paramId, value, sampleOffset);
			}
			return true;
		}

		bool AudioCompressorProcessor::processEvenets(IEventList* events) {
			// stub
			return true;
		}

		void AudioCompressorProcessor::setParameter(int index, ParamValue paramValue, int32 sampleOffset)
		{
			if (index >= ParameterIds::kNumParams) {
				return;
			}
			this->parameters[index] = paramValue;
			switch (index)
			{
			case ParameterIds::kThreshold:
				this->envelopeGenerator->setThreshold(this->parameters[index]);
				break;
			case ParameterIds::kAttack:
				this->envelopeGenerator->setAttackTime(this->parameters[index]);
				break;
			case ParameterIds::kRelease:
				this->envelopeGenerator->setReleaseTime(this->parameters[index]);
				break;
			case ParameterIds::kKnee:
				this->envelopeGenerator->setSoftKnee(this->parameters[index]);
				break;
			case ParameterIds::kOutput:
				this->makeUpGain = pow(16.0, this->parameters[index]);
				break;
			case ParameterIds::kRatio:
				this->envelopeGenerator->setRatio(this->parameters[index]);
				break;
			default:
				break;
			}
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

			LOG(Logger::kINFO, "");
			for (int i = 0; i < ParameterIds::kNumParams; i++) {
				double parameterToLoad(0);
				if (state->read(&parameterToLoad, sizeof(double)) != kResultOk) {
					LOG(Logger::kERROR, "fail to read parameter from state");
					return kResultFalse;
				}
				this->setParameter(i, static_cast<ParamValue>(parameterToLoad), 0);

				if (Logger::isLogging(Logger::kINFO)) {
					std::stringstream log;
					log << i << "," << parameterToLoad;
					LOG(Logger::kINFO, log);
				}

			}
			return kResultOk;

		}

		tresult PLUGIN_API AudioCompressorProcessor::getState(IBStream* state) {


			LOG(LogTool::Logger::kINFO, "");
			for (int i = 0; i < ParameterIds::kNumParams; i++) {
				double parameterToSave = static_cast<double>(this->parameters[i]);
#if BYTEORDER == kBigEndian
				SWAP_64(parameterToSave, sizeof(double))
#endif
				state->write(&parameterToSave, sizeof(double));

				if (Logger::isLogging(Logger::kINFO)) {
					std::stringstream log;
					log << i << "," << parameterToSave;
					LOG(Logger::kINFO, log);
				}
			}
			return kResultOk;
		}

		tresult AudioCompressorProcessor::process(ProcessData& data) 
		{
			this->processParameterChanges(data.inputParameterChanges);
			this->processEvenets(data.inputEvents);

			Sample32* inL = data.inputs[0].channelBuffers32[0];
			Sample32* inR = data.inputs[0].channelBuffers32[1];
			Sample32* outL = data.outputs[0].channelBuffers32[0];
			Sample32* outR = data.outputs[0].channelBuffers32[1];

			auto numSamples = data.numSamples;

			double cv = 1.0;
			double minCv = 1.0;
			for (int i = 0; i < numSamples; i++) {
				cv = this->envelopeGenerator->processing(inL[i], inR[i]);
				if (minCv > cv) {
					minCv = cv;
				}
				outL[i] = inL[i] * cv * makeUpGain;
				outR[i] = inR[i] * cv * makeUpGain;
			}
			this->setParameter(ParameterIds::kReduction, 1.0 - minCv, 0);
			auto outputParmeterChanges = data.outputParameterChanges;
			int32 index(0);
			auto parameterQueue = outputParmeterChanges->addParameterData(ParameterIds::kReduction, index);
			if (parameterQueue) {
				int32 index2(0);
				parameterQueue->addPoint(0, parameters[kReduction], index2);
			}
			return kResultTrue;

		}

	}
}
}
