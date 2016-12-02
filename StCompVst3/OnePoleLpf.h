#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	template<typename T>
	class OnePoleLpf {
	public:
		OnePoleLpf();
		virtual ~OnePoleLpf();
		void setSampleRate(SampleRate sampleRate);

		void setCutoffFrequency(T cutoffFrequency);
		T getCutoffFrequency();
		T processing(T input);
		void clearBuffer();

	private:
		T preWarp(T freq);
		void calcCoefficients();
		SampleRate sampleRate;
		T cutoffFrequency;
		T buffer[2];
		T coeAlpha;
		T coeBeta;

	};

}
}
}

#include "OnePoleLpfImpl.h"
