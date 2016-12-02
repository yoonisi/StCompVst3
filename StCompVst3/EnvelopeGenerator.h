#pragma once

#include "public.sdk\source\vst\vstaudioeffect.h"
#include "OnePoleLpf.h"
#include <stdint.h>
#include <memory>

namespace Steinberg {
namespace Vst {
namespace StComp {

	template<typename T>
	class EnvelopeGenerator {
	public:
		EnvelopeGenerator();
		virtual ~EnvelopeGenerator();

		T processing(T inL, T inR);
		void setReleaseTime(double release);
		void setAttackTime(double attack);
		void setThreshold(double threshold);
		void setSoftKnee(double Knee);
		void setRatio(double ratio);
		void setSampleRate(SampleRate sampleRate);
		void clearBuffer();

	private:

		inline T absOr(T inL, T inR);
		inline T thresholdCut(T input);
		inline T peakHold(T input);
		inline T calcKneeCurve(T input);
		inline void calcThresholdParamters();
		T peakBuff;
		
		uint32_t peakCount;
		T ratio;
		T outputBuffer;
		T releaseTime;
		T attackCoef;
		T attack;
		T thresholdLevel;
		T softKnee;
		T thresholdA;
		T thresholdB;
		T xOffset;
		T phai;
		T bCoef;
		T cCoef;
		T sampleRate;

		std::unique_ptr<OnePoleLpf<T> > onePoleLpf;

		const double tinyLevel;
		const double maxAttackFrequency;

	};
}
}
}

#include "EnvelopeGeneratorImpl.h"
