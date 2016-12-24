#pragma once


#include "public.sdk\source\vst\vstaudioeffect.h"

namespace Steinberg {
	namespace Vst {

			template<typename T>
			class ISimpleFilter {
			public:
				virtual void setSampleRate(SampleRate sampleRate) = 0;
				virtual void setCutoffFrequency(T frequency) = 0;
				virtual T getCutoffFrequency() = 0;
				virtual T processing(T input) = 0;
				virtual void clearBuffer() = 0;
			};


	}
}



