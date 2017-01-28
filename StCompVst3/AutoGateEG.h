#pragma once

#include <memory>
#include "public.sdk\source\vst\vstaudioeffect.h"
#include "ISimpleFilter.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	template<typename T>
	class AutoGateEG
	{
	public:
		AutoGateEG();
		virtual ~AutoGateEG(){}
		T processing(T inL, T inR);
		void setSampleRate(SampleRate sampleRate);
		void setAttackTime(double normalizedAttack);
		void setReleaseTime(double normalizedRelease);
		void setHoldTime(double normalizedHold);
		void setThreshold(double normalizedThreshold);
		void setRange(double normalizedRange);
		void setRatio(double normalizedRatio);
		void setDuckerMode(double normalizedDuckerMode);
		void clearBuffer();

	private:
		std::unique_ptr<ISimpleFilter<T> > lpf;
		SampleRate sampleRate;
		T attack;
		T release;
		T hold;
		T threshold;
		T ratio;
		T range;
		bool isDuckerMode;

		T peakBuffer;
		T outputBuffer;
		uint64_t peakCount;

		inline T absOr(T inL, T inR);
		inline T peakHold(T input);
		inline T thesholdCut(T input);
	};

}
}
}

#include "AutoGateEGImpl.h"
