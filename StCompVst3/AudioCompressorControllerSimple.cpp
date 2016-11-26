
#include <math.h>

#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/base/ustring.h"
#include "base/source/fstring.h"

#include "AudioCompressorControllerSimple.h"

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
			Steinberg::UString(info.units, USTRINGSIZE(info.title)).assign(USTRING("dB"));
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

		virtual bool fromString(const TChar* string, ParamValue& normValue) const {
			String wrapper((TChar*)string);
			double tmp = 0.;
			if (wrapper.scanFloat(tmp)) {
				if (tmp > 0.0) {
					tmp = -tmp;
				}
				normValue = expf(logf(10.f) * (float)tmp / 20.f);
				return true;
			}
			return false;
		}
	};

	tresult PLUGIN_API AudioCompressorControllerSimple::initialize(FUnknown* context) {
		tresult result = EditController::initialize(context);
		if (result != kResultOk) {
			return result;
		}

		auto thresholdParameter = new ThresholdParameter(ParameterInfo::kCanAutomate, ParameterIds::kThreshold);
		parameters.addParameter(thresholdParameter);

		return kResultOk;
	}

}
}
}
