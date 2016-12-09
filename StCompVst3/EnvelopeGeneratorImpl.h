#include "EnvelopeGenerator.h"
#include "logger.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	template<typename T>
	EnvelopeGenerator<T>::EnvelopeGenerator()
		: 
		maxAttackFrequency(16e3),
		ratio(0),
		attack(0),
		softKnee(0),
		thresholdLevel(1.0),
		thresholdQ(1.0),
		thresholdB(1.0),
		cA(0),
		cP(0),
		onePoleLpf(new OnePoleLpf<T>)
	{
		this->clearBuffer();
		this->setSampleRate(44.1e3);
		this->setAttackTime(0.1);
		this->setReleaseTime(0.5);
		this->setThreshold(1.0);
		this->setSoftKnee(0);
		
	}

	template<typename T>
	EnvelopeGenerator<T>::~EnvelopeGenerator() {
	}

	template<typename T>
	T EnvelopeGenerator<T>::processing(T inL, T inR) {
		T tmp = thresholdCut(this->onePoleLpf->processing(peakHold(absOr(inL, inR))));
		return thresholdLevel / (this->ratio * tmp + thresholdLevel);
	}

	template<typename T>
	void EnvelopeGenerator<T>::clearBuffer() {
		this->peakBuff = this->outputBuffer = 0;
		this->peakCount = 0;
		this->onePoleLpf->clearBuffer();
	}

	template<typename T>
	void EnvelopeGenerator<T>::setReleaseTime(double normalizedRelease) {
		this->releaseTime = 0.01 + 1.0 * normalizedRelease;
	}

	template<typename T>
	void EnvelopeGenerator<T>::setSampleRate(SampleRate sampleRate) {
		this->sampleRate = sampleRate;
		this->onePoleLpf->setSampleRate(sampleRate);
		this->setAttackTime(this->attack);
	}

	template<typename T>
	void EnvelopeGenerator<T>::setRatio(double ratio) {
		this->ratio = ratio;
	}


	template<typename T>
	void EnvelopeGenerator<T>::setAttackTime(double normalizedAttack) {
		this->attack = normalizedAttack;
		T frequency = 1. / ((1.0 / this->maxAttackFrequency) + 0.1 * normalizedAttack);
		this->onePoleLpf->setCutoffFrequency(frequency);
	}

	template<typename T>
	void EnvelopeGenerator<T>::setSoftKnee(double knee) {
		this->softKnee = 0.001 * pow(1000,knee);
		this->calcThresholdParamters();
	}

	template<typename T>
	void EnvelopeGenerator<T>::setThreshold(double threshold) {
		this->thresholdLevel = 0.001 * pow(1000, threshold);
		this->calcThresholdParamters();
	}

	template<typename T>
	inline void EnvelopeGenerator<T>::calcThresholdParamters() {
		using namespace LogTool;
		this->thresholdQ = this->thresholdLevel;
		this->thresholdB = this->thresholdLevel * this->softKnee;
		this->cP =
			2. * this->thresholdQ + 
			sqrt(
				(2.*this->thresholdQ)*(2.*this->thresholdQ) 
				- 4.*(2. * this->thresholdB*this->thresholdQ - this->thresholdB*this->thresholdB));
		this->cP /= 2.;
		T diviser = 2. * (this->thresholdB - this->cP);
		if (diviser == 0.0) { diviser = 1e-6; }
		this->cA = 1 / diviser;
		return;
	}

	template<typename T>
	T EnvelopeGenerator<T>::calcKneeCurve(T input) {
		return this->phai * input * input + this->bCoef * input + this->cCoef;
	}

	template<typename T>
	inline T EnvelopeGenerator<T>::absOr(T inL, T inR) {
		T tmpL = inL < 0. ? -inL : inL;
		T tmpR = inR < 0. ? -inR : inR;
		T result = tmpL < tmpR ? tmpR : tmpL;
		return result;
	}

	template<typename T>
	inline T EnvelopeGenerator<T>::thresholdCut(T input) {
		T result = 0.;
		if (input < this->thresholdB) {
			;
		} else if (input < this->cP){
			result = input - (this->cA*(input - this->cP)*(input - this->cP) + this->thresholdQ);
		} else {
			result = input - this->thresholdQ;
		}
		return result;
	}

	template<typename T>
	T EnvelopeGenerator<T>::peakHold(T input) {
		if (this->outputBuffer < input) {
			this->peakBuff = this->outputBuffer = input;
			this->peakCount = 0;
		}
		else {
			if (static_cast<T>(this->peakCount) <= static_cast<T>(this->sampleRate * 10)) {
				this->outputBuffer = this->peakBuff *
					pow(M_E, -(static_cast<T>(this->peakCount) / this->sampleRate) / this->releaseTime);
				this->peakCount++;
			}
		}
		return this->outputBuffer;
	}

}
}
}
