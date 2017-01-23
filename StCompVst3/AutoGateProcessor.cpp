#include "AutoGateProcessor.h"
#include "AutoGateConsts.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/base/ibstream.h"
#include "AutoGateParameters.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	AutoGateProcessor::AutoGateProcessor() :
		sampleRate(44.1e3),
		lowPassFilter(new IirLpf<double>[2]),
		highPassFilter(new IirHpf<double>[2]),
		envelopeGenerator(new AutoGateEG<double>()),
		parameters(new ParamValue[ParameterIds::kNumParams])
	{

		this->lowPassFilter[0].setCutoffFrequency(20e3);
		this->lowPassFilter[1].setCutoffFrequency(20e3);
		this->highPassFilter[0].setCutoffFrequency(20);
		this->highPassFilter[1].setCutoffFrequency(20);

		for (int i = 0; i < ParameterIds::kNumParams; i++) {
			switch (i)
			{
			case ParameterIds::kKeyLpfCutoff:
				this->setParameter(i, 1.0, 0);
				break;
			case ParameterIds::kRelease:
				this->setParameter(i, 0.1, 0);
				break;
			default:
				this->setParameter(i, 0.0, 0);
				break;
			}
		}

	}

	AutoGateProcessor::~AutoGateProcessor(){}

	tresult PLUGIN_API AutoGateProcessor::initialize(FUnknown* context) {
		auto result = AudioEffect::initialize(context);
		if (result == kResultTrue) {
			addAudioInput(STR16("AudioInput"), SpeakerArr::kStereo);
			addAudioOutput(STR16("AudioOutput"), SpeakerArr::kStereo);
		}
		return result;
	}

	tresult PLUGIN_API AutoGateProcessor::terminate()
	{
		return AudioEffect::terminate();
	}

	tresult PLUGIN_API AutoGateProcessor::canProcessSampleSize(int32 symbolicSampleSize) {
		if (symbolicSampleSize == kSample32) {
			return kResultTrue;
		}
		else if (symbolicSampleSize == kSample64) {
			return kResultTrue;
		}
		else {
			return kResultFalse;
		}
	}

	tresult PLUGIN_API AutoGateProcessor::setBusArrengements(SpeakerArrangement * inputs, int32 numInputs, SpeakerArrangement * outputs, int32 numOutputs)
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
//				return AudioEffect::setBusArrangements(inputs, numInputs, outputs, numOutputs);
				return kResultFalse;
			}
		}
		return kResultFalse;
	}

	tresult PLUGIN_API AutoGateProcessor::setState(IBStream * state)
	{
		for (int i = 0; i < ParameterIds::kNumParams; i++) {
			double parameterToLoad(0);
			if (state->read(&parameterToLoad, sizeof(double)) != kResultOk) {
				return kResultFalse;
			}
			this->setParameter(i, static_cast<ParamValue>(parameterToLoad), 0);

		}
		return kResultOk;
	}

	tresult PLUGIN_API AutoGateProcessor::getState(IBStream * state)
	{

		for (int i = 0; i < ParameterIds::kNumParams; i++) {
			double parameterToSave = static_cast<double>(this->parameters[i]);
#if BYTEORDER == kBigEndian
			SWAP_64(parameterToSave, sizeof(double))
#endif
				state->write(&parameterToSave, sizeof(double));
		}
		return kResultOk;
	}

	tresult PLUGIN_API AutoGateProcessor::process(ProcessData & data)
	{
		this->processParameterChanges(data.inputParameterChanges);
		this->processEvenets(data.inputEvents);
		auto numSamples = data.numSamples;

		auto numChannels = data.inputs[0].numChannels;

		return kResultTrue;
	}

	tresult PLUGIN_API AutoGateProcessor::setupProcessing(ProcessSetup& setup) {
		this->sampleRate = setup.sampleRate;
		
		this->lowPassFilter[0].setSampleRate(this->sampleRate);
		this->lowPassFilter[1].setSampleRate(this->sampleRate);
		this->highPassFilter[0].setSampleRate(this->sampleRate);
		this->highPassFilter[1].setSampleRate(this->sampleRate);
		return AudioEffect::setupProcessing(setup);

	}

	bool AutoGateProcessor::processParameterChanges(IParameterChanges* changes) {
		if (!changes) { return false; }
		int32 count = changes->getParameterCount();
		if (count == 0) { return false; }
		for (int i = 0; i < count; i++) {
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

	bool AutoGateProcessor::processEvenets(IEventList* events) {
		return true;
	}

	void AutoGateProcessor::setParameter(int index, ParamValue paramValue, int32 sampleOffset) {
		if (index >= ParameterIds::kNumParams) {
			return;
		}
		this->parameters[index] = paramValue;
		switch (index)
		{
		case ParameterIds::kKeyLpfCutoff:
			this->lowPassFilter[0].setCutoffFrequency(LpfCutoffParameter::valueConvert(parameters[index]));
			this->lowPassFilter[1].setCutoffFrequency(LpfCutoffParameter::valueConvert(parameters[index]));
			break;
		case ParameterIds::kKeyHpfCutoff:
			this->highPassFilter[0].setCutoffFrequency(HpfCutoffParameter::valueConvert(parameters[index]));
			this->highPassFilter[1].setCutoffFrequency(HpfCutoffParameter::valueConvert(parameters[index]));
			break;
		default:
			break;
		}
	}

	template<typename T>
	inline void AutoGateProcessor::audioProcessing(ProcessData & data, int samples, T * inL, T * inR, T * outL, T * outR)
	{
		if (parameters[ParameterIds::kBypassProcess] > 0.5) {
			if (inL != outL) {
				for (int i = 0; i < samples; i++) {
					outL[i] = inL[i];
				}
			}
			if (inR != outR) {
				for (int i = 0; i < samples; i++) {
					outR[i] = inR[i];
				}
			}
		}
		else {
			for (int i = 0; i < samples; i++) {
				// pre fileter
				double keyL = this->highPassFilter[0].processing(inL[i]);
				KeyL = this->lowPassFilter[0].processing(keyL);
				double keyR = this->highPassFilter[1].processing(inR[i]);
				KeyR = this->lowPassFilter[1].processing(keyR);

				// envelope generator

				if (parameters[ParameterIds::kKeyListen] > 0.5) {
					outL[i] = keyL;
					outR[i] = keyR;
				}
				else {
					outL[i] = inL[i];
					outR[i] = inR[i];
				}
			}
		}
	}

	template<typename T>
	void AutoGateProcessor::audioProcessingMono(ProcessData & data, int samples, T * in, T * out)
	{
	}


}
}
}
