#pragma once

#include "IirHpf.h"

#include "DspUtils.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace Steinberg {
namespace Vst {

	template<typename T>
	IirHpf<T>::IirHpf() :
		sampleRate(44.1e3),
		cutoffFrequency(100),
		cutoffQ(0.707)
	{
		this->clearBuffer();
		this->calcCoef();
	}

	template<typename T>
	void IirHpf<T>::clearBuffer() {
		for (int i = 0; i < 4; i++) {
			this->filterBuffer[i] = 0.0;
		}
	}

	template<typename T>
	void IirHpf<T>::setCutoffFrequency(T cutoffFrequency) {
		this->cutoffFrequency = cutoffFrequency;
		this->calcCoef();
	}

	template<typename T>
	T IirHpf<T>::getCutoffFrequency() {
		return this->cutoffFrequency;
	}

	template<typename T>
	void IirHpf<T>::calcCoef() {
		double cutoff = DspUtils::prewarpFrequency(this->cutoffFrequency, this->sampleRate);
		double omega = 2. * M_PI * cutoff / this->sampleRate;
		double si = sin(omega);
		double co = cos(omega);
		double alpha = si / (2. * this->cutoffQ);

		double a[3], b[3];
		b[0] = (1.0 + co) / 2.0;
		b[1] = -(1.0 + co);
		b[2] = b[0];
		a[0] = 1.0 + alpha;
		a[1] = -2.0 * co;
		a[2] = 1.0 - alpha;
		this->filterCoef[0] = b[0] / a[0];
		this->filterCoef[1] = b[1] / a[0];
		this->filterCoef[2] = b[2] / a[0];
		this->filterCoef[3] = a[1] / a[0];
		this->filterCoef[4] = a[2] / a[0];
	}


	template<typename T>
	void IirHpf<T>::setSampleRate(SampleRate sampleRate) {
		this->sampleRate = sampleRate;
		this->calcCoef();
		this->clearBuffer();
	}

	template<typename T>
	T IirHpf<T>::processing(T input) {
		DspUtils::undenormalize(input);
		T result =
			this->filterCoef[0] * input +
			this->filterCoef[1] * this->filterBuffer[0] +
			this->filterCoef[2] * this->filterBuffer[1] -
			this->filterCoef[3] * this->filterBuffer[2] -
			this->filterCoef[4] * this->filterBuffer[3];
		DspUtils::undenormalize(result);
		this->filterBuffer[3] = this->filterBuffer[2];
		this->filterBuffer[2] = result;
		this->filterBuffer[1] = this->filterBuffer[0];
		this->filterBuffer[0] = input;
		return result;
	}
}
}


