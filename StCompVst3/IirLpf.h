#pragma once

#include "ISimpleFilter.h"

namespace Steinberg {
namespace Vst {

	template<typename T>
	class IirLpf : public ISimpleFilter<T> 
	{
	public:
		IirLpf();
		virtual ~IirLpf(){}
		virtual void setSampleRate(SampleRate sampleRate);
		virtual void setCutoffFrequency(T frequency);
		virtual T getCutoffFrequency();
		virtual T processing(T input);
		virtual void clearBuffer();

	protected:
		SampleRate sampleRate;
		T filterCoef[5];
		T filterBuffer[4];
		T cutoffFrequency;
		T cutoffQ;
		virtual void calcCoef();

	};


}
}

#include "IirLpfImpl.h"
