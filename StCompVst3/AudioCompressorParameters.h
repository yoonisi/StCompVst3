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
		ThresholdParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Threshold"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 1.0f;
			info.unitId = unitID;
			setNormalized(1.0f);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}
		static void stringConvert(float value, char* string) {
			sprintf(string, "%.1f dB", static_cast<float>(-60. * (1.0 - value)));
		}
		
	}; 
	
	class RatioParameter : public Parameter
	{
	public:
		RatioParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
		Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Ratio"));
		Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
		info.flags = flags;
		info.id = id;
		info.stepCount = 0;
		info.defaultNormalizedValue = 0.0f;
		info.unitId = unitID;
		setNormalized(0.0f);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}
		static void stringConvert(float value, char* string) {
			if (value == 1.0f){
				strcpy(string, "oo:1");
			}
			else {
				float ratio = 1.0f / (1.0f - value);
				if (ratio < 100) {
					sprintf(string, "%.1f:1", ratio);
				}
				else {
					strcpy(string, "oo:1");
				}
			}
		}
		
	};

	class AttackParameter : public Parameter 
	{
	public:
		AttackParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Attack"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0.2;
			info.unitId = unitID;
			setNormalized(0.2);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

		static void stringConvert(float value, char* string) {
			sprintf(string, "%.1f ms", 0.1f + 99.9f * value);
		}
		
	};

	class ReleaseParameter : public Parameter
	{
	public:
		ReleaseParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Release"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0.5;
			info.unitId = unitID;
			setNormalized(0.5);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

		static void stringConvert(float value, char* string) {
			sprintf(string, "%.1f ms", 10.f + (990.f * value));
		}
		
	};

	class OutputParameter : public Parameter {
	public:
		OutputParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Output"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.unitId = unitID;
			setNormalized(0.0);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

		static void stringConvert(float value, char* string) {
			sprintf(string, "%.1f dB", 24.f * value);
		}
		
	};

	class KneeParameter : public Parameter {
	public:
		KneeParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Knee"));
			Steinberg::UString(info.units, USTRINGSIZE(info.units)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 1.0;
			info.unitId = unitID;
			setNormalized(0.0);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			sprintf(text, "%.1f", static_cast<float>(1.-normValue));
			Steinberg::UString(label, 128).fromAscii(text);
			
		}
		
	};

	class ReductionParameter : public Parameter {
	public:
		ReductionParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Reduction"));
			Steinberg::UString(info.units, USTRINGSIZE(info.title)).assign(USTRING(""));
			info.flags = flags;
			info.id = id;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.unitId = unitID;
			setNormalized(0.0);
		}
		
		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			sprintf(text, "%.2f", static_cast<float>(normValue));
			Steinberg::UString(label, 128).fromAscii(text);
		}
		
	};

	class BypassParameter : public Parameter {
	public:
		BypassParameter(int32 flags, int32 id, UnitID unitID = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Bypass"));
			info.flags = flags | ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass;
			info.id = id;
			info.stepCount = 1;
			info.defaultNormalizedValue = 0;
			info.unitId = unitID;
			setNormalized(0.0);
		}
	};



}
}
}
