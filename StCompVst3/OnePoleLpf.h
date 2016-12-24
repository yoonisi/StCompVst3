#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"
#include "ISimpleFilter.h"

namespace Steinberg {
namespace Vst {


	template<typename T>
	class OnePoleLpf : public ISimpleFilter<T> {
	public:
		OnePoleLpf();
		virtual ~OnePoleLpf();
		void setSampleRate(SampleRate sampleRate);

		void setCutoffFrequency(T cutoffFrequency);
		T getCutoffFrequency();
		T processing(T input);
		void clearBuffer();

	private:
		void calcCoefficients();
		SampleRate sampleRate;
		T cutoffFrequency;
		T buffer[2];
		T coeAlpha;
		T coeBeta;

	};


}
}

#include "OnePoleLpfImpl.h"
