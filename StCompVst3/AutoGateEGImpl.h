#pragma once

#include "AutoGateEG.h"
#include "OnePoleLpf.h"
#include "ISimpleFilter.h"
#include "AutoGateParameters.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	template<typename T>
	AutoGateEG<T>::AutoGateEG() 
		: 
		lpf(new OnePoleLpf<double>()),
		attack(0.5e-3),
		release(100e-3),
		hold(100e-3),
		threshold(1e-6),
		ratio(1.0),
		isDuckerMode(false),
		sampleRate(44.1e3),
		peakCount(0),
		peakBuffer(0)
	{
		this->clearBuffer();
		this->setSampleRate(44.1e3);
		this->setAttackTime(0.01);
		this->setReleaseTime(0.1);
		this->setHoldTime(0.1);
		this->setThreshold(0);
		this->setRatio(0);
		this->setDuckerMode(0);
	}

	template<typename T>
	void AutoGateEG<T>::clearBuffer() {
		this->peakCount = 0;
		this->peakBuffer = 0;
		this->lpf->clearBuffer();
	}

	template<typename T>
	void AutoGateEG<T>::setAttackTime(double normalizeAttack) {
		this->attack = AttackParameter::valueConvert(normalizeAttack);
	}

	template<typename T>
	void AutoGateEG<T>::setReleaseTime(double normalizedRelease) {
		this->release = ReleaseParameter::valueConvert(normalizedRelease);
	}

	template<typename T>
	void AutoGateEG<T>::setRange(double normalizedRange) {
		this->range = RangeParameter::valueConvert(normalizedRange);
	}

	template<typename T>
	void AutoGateEG<T>::setHoldTime(double normalizedHold) {
		this->hold = HoldParameter::valueConvert(normalizedHold);
	}

	template<typename T>
	void AutoGateEG<T>::setThreshold(double normalizedThreshold) {
		this->threshold = ThresholdParameter::valueConvert(normalizedThreshold);
	}

	template<typename T>
	void AutoGateEG<T>::setRatio(double normalizedRatio) {
		this->ratio = RatioParameter::valueConvert(normalizedRatio);
	}

	template<typename T>
	void AutoGateEG<T>::setDuckerMode(double normalzedValue) {
		this->isDuckerMode = (normalzedValue > 0.5) ? true : false;
	}

	template<typename T>
	void AutoGateEG<T>::setSampleRate(SampleRate sampleRate) {
		this->sampleRate = sampleRate;
		this->lpf->setSampleRate(sampleRate);
		this->clearBuffer();
	}

	template<typename T>
	inline T AutoGateEG<T>::absOr(T inR, T inL) {
		T absL = inL < 0 ? -inL : inL;
		T absR = inR < 0 ? -inR : inR;
		return (absL > absR) ? absL : absR;
	}

	

	template<typename T>
	inline T AutoGateEG<T>::peakHold(T input)
	{
		if ((this->outputBuffer < input) && (this->threshold < input)) { // get peak
			this->peakBuff = this->outputBuffer = input;
			this->peakCount = 0;
		}
		else {
			double envelopeTime = static_cast<T>(this->peakCount) * (1.0 / this->sampleRate);
			bool isHold = (envelopeTime <= this->hold);
			bool isRelease = false;
			if (envelopeTime <= this->hold) {
				isRelease = false;
			}
			else if ((envelopeTime - this->hold) <= (this->release * 20.0)) {
				isRelease = true;
			}
			else {
				isRelease = false;
			}

			if (isHold)
			{
				this->outputBuffer = this->peakBuffer;
			} else if (isRelease)
			{
				this->outputBuffer = this->peakBuff *
					pow(M_E, -(static_cast<T>(this->peakCount) / this->sampleRate) / this->releaseTime);
			}
			else {
				this->outputBuffer = 0;
			}
			this->peakCount++;
		}
		return this->outputBuffer;
	}

	template<typename T>
	inline T AutoGateEG<T>::processing(T inL, T inR)
	{
		T input = absOr(inR, inL);
		T envelope = this->peakHold(lpfout);
		T normalizedEnvelope = envelope / this->threshold;
		T gateGain = thesholdCut(normalizedEnvelope);
		T gate = this->lpf->processing(gate);
		return 0;
	}

	template<typename T>
	inline T AutoGateEG<T>::thesholdCut(T input) {
		T normalizedEnvelope = T / this->setThreshold;
		if (normalizedEnvelope < 1.0) {
			// minimum gain range scaling
			T maxRange = 1.0 - this->range;
			T rangedEnvelope = normalizedEnvelope * maxRange + this->range;
			return rangedEnvelope;
		}
		else {
			// expander
			T overGain = normalizedEnvelope - 1.0;
			overGain = overGain * (this->ratio - 1.0)/*0.0 - 29*/;
			return overGain + 1.0;
		}
	}

}
}
}
