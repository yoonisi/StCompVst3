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

		T processing(T inL, T inR, double ratio);
		void setReleaseTime(double release);
		void setAttackTime(double attack);
		void setThreshold(double threshold);
		void calcThresholdParamters();
		void setSoftKnee(double Knee);
		void setSampleRate(SampleRate sampleRate);
		void clearBuffer();

	private:

		inline T absOr(T inL, T inR);
		inline T thresholdCut(T input);
		inline T peakHold(T input);
		inline T calcKneeCurve(T input);
		T peekBuff;
		T lpfBuff;
		
		uint32_t peakCount;
		T outputBuffer;
		T releaseTime;
		T attackCoef;
		T attack;
		T attackFrequency;
		T thresholdLevel;
		T softKnee;
		T thresholdA;
		T thresholdB;
		T xOffset;
		T phai;
		T bCoef;
		T cCoef;
		T sampleRate;
		T sampleRate10;
		T sampleRate05;

		std::unique_ptr<OnePoleLpf<T> > onePoleLpf;

		const double subLpfCutoff;
		const double tinyLevel;
		const double maxAttackFrequency;

	};
}
}
}

#include "EnvelopeGeneratorImpl.h"
