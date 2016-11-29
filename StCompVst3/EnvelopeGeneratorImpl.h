#include "EnvelopeGenerator.h"


namespace Steinberg {
namespace Vst {
namespace StComp {

	template<typename T>
	EnvelopeGenerator<T>::EnvelopeGenerator()
		: 
		subLpfCutoff(17.119e3),
		tinyLevel(-0.005),
		maxAttackFrequency(16e3),
		onePoleLpf(new OnePoleLpf<T>)
	{
		this->clearBuffer();
		this->attack = 0;
		this->setSampleRate(44.1e3);
	}

	template<typename T>
	EnvelopeGenerator<T>::~EnvelopeGenerator() {
	}

	template<typename T>
	T EnvelopeGenerator<T>::processing(T inL, T inR, double ratio) {
		T tmp = thresholdCut(this->onePoleLpf->processing(peakHold(absOr(inL, inR))));
		return thresholdLevel / (ratio * tmp + thresholdLevel);
	}

	template<typename T>
	void EnvelopeGenerator<T>::clearBuffer() {
		this->peekBuff = this->lpfBuff = this->outputBuffer = 0;
		this->peakCount = 0;
		this->onePoleLpf->clearBuffer();
	}

	template<typename T>
	void EnvelopeGenerator<T>::setReleaseTime(double normalizedRelease) {
		this->rele
			aseTime = 0.01 + 1.0 * releaseSec;
	}

	template<typename T>
	void EnvelopeGenerator<T>::setSampleRate(SampleRate sampleRate) {
		this->sampleRate = sampleRate;
		this->sampleRate10 = sampleRate * 10.0;
		this->sampleRate05 = sampleRate05 / 2.0;
		this->setAttackTime(this->attack);
		this->onePoleLpf->setSampleRate(sampleRate);
		this->onePoleLpf->setCutoffFrequency(attackFrequency);
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
	}

	template<typename T>
	void EnvelopeGenerator<T>::setThreshold(double threshold) {
		this->thresholdLevel = 0.001 * pow(1000, threshold);
	}

	template<typename T>
	void EnvelopeGenerator<T>::calcThresholdParamters() {
		this->thresholdB = this->thresholdLevel;
		this->thresholdA = this->softKnee * this->thresholdLevel - tinyLevel;
		this->xOffset = 2. * this->thresholdB - this->thresholdA;
		this->phai = 1.0 / (4.0 * (this->thresholdA - this->thresholdB));
		this->bCoef = -2.0 * this->xOffset * this->phai;
		this->cCoef = this->phai * this->xOffset * this->xOffset + this->thresholdB;
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
		if (input < this->thresholdA) {
			; // return 0
		}
		else if (this->thresholdA <= input && input <= this->xOffset) {
			result = input - this->calcKneeCurve(input);
		}
		else if (this->xOffset < input) {
			result = input - this->thresholdB;
		}
		return result;
	}

	template<typename T>
	T EnvelopeGenerator<T>::peakHold(T input) {
		if (this->outputBuffer < input) {
			this->peekBuff = this->outputBuffer = input;
			this->peakCount = 0;
		}
		else {
			if (static_cast<T>(this->peakCount) <= this->sampleRate10) {
				this->outputBuffer = this->peekBuff *
					pow(M_E, -(static_cast<T>(this->peakCount) / this->sampleRate) / this->releaseTime);
				this->peakCount++;
			}
		}
		return this->outputBuffer;
	}

}
}
}
