#include "EnvelopeGenerator.h"
#include "logger.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	template<typename T>
	EnvelopeGenerator<T>::EnvelopeGenerator()
		: 
		tinyLevel(-0.005),
		maxAttackFrequency(16e3),
		ratio(0),
		attack(0),
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
		this->softKnee = (1.0 - tinyLevel)*(knee * knee) + tinyLevel;
		this->calcThresholdParamters();
	}

	template<typename T>
	void EnvelopeGenerator<T>::setThreshold(double threshold) {
		this->thresholdLevel = 0.001 * pow(1000, threshold);
		this->calcThresholdParamters();
	}

	template<typename T>
	inline void EnvelopeGenerator<T>::calcThresholdParamters() {
		this->thresholdB = this->thresholdLevel;
		this->thresholdA = this->softKnee * this->thresholdLevel - tinyLevel;
		this->xOffset = 2. * (this->thresholdB - this->thresholdA);
		this->phai = 1.0 / (4.0 * (this->thresholdA - this->thresholdB));
		this->bCoef = -2.0 * this->xOffset * this->phai;
		this->cCoef = this->phai * this->xOffset * this->xOffset + this->thresholdB;
		if (Logger::isLogging(Logger::kINFO)) {
			stringstream str;
			str << "thresholdB," << this->thresholdB << ",thresholdA," << this->thresholdA << ",xOffset," << this->xOffset;
			LOG(Logger::kINFO, str);
		}
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
		} else {
			// tentative (not implemented soft knee)
			result = input - this->thresholdB;
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
