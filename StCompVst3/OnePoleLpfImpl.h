
#define _USE_MATH_DEFINES

#include <math.h>
#include "OnePoleLpf.h"
#include "DspUtils.h"

namespace Steinberg {
namespace Vst {

	template<typename T>
	OnePoleLpf<T>::OnePoleLpf() : sampleRate(44.1e3), cutoffFrequency(1e3) {
		clearBuffer();
		calcCoefficients();
	}

	template<typename T>
	OnePoleLpf<T>::~OnePoleLpf(){}

	template<typename T>
	void OnePoleLpf<T>::setSampleRate(SampleRate sampleRate) {
		this->sampleRate = sampleRate;
		calcCoefficients();
	}

	template<typename T>
	void OnePoleLpf<T>::setCutoffFrequency(T cutoffFrequency) {
		this->cutoffFrequency = cutoffFrequency;
		calcCoefficients();
	}

	template<typename T>
	T OnePoleLpf<T>::getCutoffFrequency() {
		return cutoffFrequency;
	}

	template<typename T>
	T OnePoleLpf<T>::processing(T input) {
		DspUtils::undenormalize(input);
		T result = this->coeAlpha * (input + this->buffer[0]) + this->coeBeta * this->buffer[1];
		DspUtils::undenormalize(result);
		this->buffer[0] = input;
		this->buffer[1] = result;
		return result;
	}

	template<typename T>
	void OnePoleLpf<T>::clearBuffer() {
		this->buffer[0] = this->buffer[1] = 0;
	}

	template<typename T>
	void OnePoleLpf<T>::calcCoefficients() {
		T omegaC = DspUtils::prewarpFrequency<double>(this->cutoffFrequency, this->sampleRate);
		T omegaS = 2. * M_PI * this->sampleRate;
		this->coeAlpha = (M_PI * omegaC) / (M_PI * omegaC + omegaS);
		this->coeBeta = (omegaS - M_PI * omegaC) / (M_PI * omegaC + omegaS);
		return;
	}


}
}
