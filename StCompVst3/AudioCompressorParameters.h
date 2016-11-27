#pragma once


#include <math.h>


#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "base/source/fstring.h"
#include "AudioCompressorConsts.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	///////////////////////////////////////////////////
	// Parameter Classes
	///////////////////////////////////////////////////
	class ThresholdParameter : public Parameter {
	public:
		ThresholdParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Threshold"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("dB"));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 1.0f;
			info.unitId = kRootUnitId;
			setNormalized(1.0f);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			if (normValue > 1e-4) {
				sprintf(text, "%.2f", 20. * log10f((float)normValue));
			}
			else {
				strcpy(text, "-oo");
			}
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};
	
	class RatioParameter : public Parameter
	{
	public:
		RatioParameter(int32 flags, int32 id) {
		Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Ratio"));
		Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("n:1"));
		info.flags = flags;
		info.id = id;
		info.stepCount = 0;
		info.defaultNormalizedValue = 0.0f;
		info.unitId = kRootUnitId;
		setNormalized(0.0f);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			float ratioParam = (float)normValue;
			if (ratioParam == 1.0f) {
				strcpy(text, "oo");
			}
			else {
				sprintf(text, "%.2f", 1.0f / (1.0f - ratioParam));
			}
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};

	class AttackParameter : public Parameter 
	{
	public:
		AttackParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Attack"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("msec"));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0.2;
			info.unitId = kRootUnitId;
			setNormalized(0.2);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			float attackParam = (float)normValue;
			sprintf(text, "%.2f", 0.1f + 99.9f * attackParam);
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};

	class ReleaseParameter : public Parameter
	{
	public:
		ReleaseParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Release"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("msec"));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0.5;
			info.unitId = kRootUnitId;
			setNormalized(0.5);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			float releaseParam = static_cast<float>(normValue);
			sprintf(text, "%.2f", 25.f + (500.f - 25.f)*releaseParam);
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};

	class OutputParameter : public Parameter {
	public:
		OutputParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Output"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING("dB"));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.unitId = kRootUnitId;
			setNormalized(0.0);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			sprintf(text, "%.2f", static_cast<float>(ConstantValues::MaximumMakeupGain() * normValue));
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};

	class KneeParameter : public Parameter {
	public:
		KneeParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Knee"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.unitId = kRootUnitId;
			setNormalized(0.0);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			sprintf(text, "%.2f", static_cast<float>(normValue));
			Steinberg::UString(label, 128).fromAscii(text);
			
		}
	};

	class ReductionParameter : public Parameter {
	public:
		ReductionParameter(int32 flags, int32 id) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Reduction"));
			Steinberg::UString(info.units, USTRINGSIZE(info.title)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.unitId = kRootUnitId;
			setNormalized(0.0);
		}
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			sprintf(text, "%.2f", static_cast<float>(normValue));
			Steinberg::UString(label, 128).fromAscii(text);
		}
	};

}
}
}
