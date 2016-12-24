#pragma once

#include <stdint.h>
#define _USE_MATH_DEFINES
#include <math.h>

namespace DspUtils 
{
	
	inline void undenormalize(float & value) {
		if (((*(uint32_t*)&value) & 0x7f800000) == 0)
			value = 0.0f;
	}

	inline void undenormalize(double & value) {
		if (((*(uint64_t*)&value) & 0x7FF0000000000000LL) == 0)
			value = 0.0;
	}

	template<typename T>
	inline T prewarpFrequency(T frequency, T sampleRate) {
		T tau = 1.0 / sampleRate;
		T omega = 2 * M_PI * frequency;
		return 2.0 / tau * tan(omega * tau / 2);
	}

}
