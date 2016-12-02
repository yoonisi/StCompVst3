#pragma once

#include <stdint.h>

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

}
