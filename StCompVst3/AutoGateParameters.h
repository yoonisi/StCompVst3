#pragma once


#include <math.h>


#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "base/source/fstring.h"

#include "AutoGateConsts.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

	class ThresholdParameter : public Parameter
	{
	public:
		ThresholdParameter(uint32 id, UnitID uid = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Threshod"));
			info.flags = ParameterInfo::kCanAutomate;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0.0;
			info.id = id;
			info.unitId = uid;
			setNormalized(0.0);
		}

		static double valueConvert(double normalizeParameter) {
			const double minThresh = 0.001;
			return minThresh * pow(1.0/minThresh, normalizeParameter);
		}

		static void stringConvert(float value, char* string) {
			double dB = 20 * log10(valueConvert(value));
			sprintf(string, "%.1f dB", (float)dB);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

	};

	class AttackParameter : public Parameter {
	public:
		AttackParameter(int32 id, UnitID uid = kRootUnitId) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Attack"));
			info.flags = ParameterInfo::kCanAutomate;
			info.stepCount = 0;
			info.defaultNormalizedValue = 0;
			info.id = id;
			info.unitId = uid;
			setNormalized(0);
		}

		static double valueConvert(double normalizedValie) {
			const double minAttack = 0.05e-3;
			const double maxAttack = 100e-3;
			return (maxAttack - minAttack) * normalizedValie + minAttack;
		}

		static void stringConvert(float value, char* string) {
			double attackTime = valueConvert(value);
			sprintf(string, "%.1f msec", (float)(attackTime * 1e3));
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

	};

	class HoldParameter : public Parameter {
	public:
		HoldParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Hold"));
			info.flags = ParameterInfo::kCanAutomate;
			info.defaultNormalizedValue = 0;
			info.id = id;
			info.unitId = uid;
			info.stepCount = 0;
			setNormalized(0);
		}

		static double valueConvert(double normalizedValue) {
			const double maxHoldTime = 1.0;
			return normalizedValue * maxHoldTime;
		}

		static void stringConvert(float value, char* string) {
			double holdTime = valueConvert(value);
			sprintf(string, "%.1f msec", (float)(holdTime * 1e3));
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

	};

	class ReleaseParameter : public Parameter
	{
	public:
		ReleaseParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Hold"));
			info.flags = ParameterInfo::kCanAutomate;
			info.id = id;
			info.unitId = uid;
			info.defaultNormalizedValue = 0.1;
			info.stepCount = 0;
			setNormalized(0.1);
		}

		static double valueConvert(double normalizedValue) {
			const double minReleaseTime = 10e-3;
			const double maxReleaseTime = 2;
			return (maxReleaseTime - minReleaseTime) * normalizedValue + minReleaseTime;
		}

		static void stringConvert(float value, char* string) {
			sprintf(string, "%.1f msec", valueConvert(value) * 1e3);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}


	};

	class RatioParameter : public Parameter
	{
	public:
		RatioParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Expand Ratio"));
			info.defaultNormalizedValue = 0.0;
			info.flags = ParameterInfo::kCanAutomate;
			info.id = id;
			info.unitId = uid;
			info.stepCount = 0;
			setNormalized(0.0);
		}

		static double valueConvert(double normalizedValue) {
			const double maxExpandRatio = 30;
			double expandRatio = 1;
			double invNormalizedValue = 1.0 - normalizedValue;
			if (invNormalizedValue < (1/expandRatio)) {
				expandRatio = maxExpandRatio;
			}
			else {
				expandRatio = 1 / invNormalizedValue;
			}
			return expandRatio;
		}

		static void stringConvert(float value, char* string) {
			float expandRatio = valueConvert(value);
			sprintf(string, "%.1f : 1", expandRatio);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

	};

	class KeyListendParameter : public Parameter {
	public:
		KeyListendParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Key Listen"));
			info.defaultNormalizedValue = 0;
			info.flags = ParameterInfo::kCanAutomate;
			info.id = id;
			info.unitId = uid;
			info.stepCount = 1;
			setNormalized(0);
		}

	};

	class DuckerParameter : public Parameter {
	public:
		DuckerParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Ducker Mode"));
			info.defaultNormalizedValue = 0;
			info.flags = ParameterInfo::kCanAutomate;
			info.id = id;
			info.unitId = uid;
			info.stepCount = 1;
			setNormalized(0);
		}
	};

	class LpfCutoffParameter : public Parameter {
	public:
		LpfCutoffParameter(int32 id, UnitID uid) {
			Steinberg::UString(info.title, USTRINGSIZE(info.title)).assign(USTRING("Key LPF Cutoff"));
			info.defaultNormalizedValue = 1.0;
			info.flags = ParameterInfo::kCanAutomate;
			info.id = id;
			info.unitId = uid;
			info.stepCount = 0;
			setNormalized(1.0);
		}

		static double valueConvert(double normalizedValue) {
			const double maxCutoffFrequency = 20e3;
			const double minCutoffFrequency = 20;
			double biParam = normalizedValue * normalizedValue;
			return (maxCutoffFrequency - minCutoffFrequency) * biParam + minCutoffFrequency;
		}

		static void stringConvert(float value, char* string) {
			double frequency = valueConvert(value);
			sprintf(string, "%.1f Hz", frequency);
		}

		virtual void toString(ParamValue normValue, String128 label) const {
			char text[32] = { 0 };
			stringConvert(normValue, text);
			Steinberg::UString(label, 128).fromAscii(text);
		}

	};

	typedef LpfCutoffParameter HpfCutoffParameter;

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


