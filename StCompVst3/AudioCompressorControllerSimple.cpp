
#include <math.h>
#include "AudioCompressorParameters.h"
#include "AudioCompressorControllerSimple.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	tresult PLUGIN_API AudioCompressorControllerSimple::initialize(FUnknown* context) {
		tresult result = EditController::initialize(context);
		if (result != kResultOk) {
			return result;
		}

		auto thresholdParameter = new ThresholdParameter(ParameterInfo::kCanAutomate, ParameterIds::kThreshold);
		parameters.addParameter(thresholdParameter);
		auto ratioParameter = new RatioParameter(ParameterInfo::kCanAutomate, ParameterIds::kRatio);
		parameters.addParameter(ratioParameter);
		auto attackParameter = new AttackParameter(ParameterInfo::kCanAutomate, ParameterIds::kAttack);
		parameters.addParameter(attackParameter);
		auto releaseParameter = new ReleaseParameter(ParameterInfo::kCanAutomate, ParameterIds::kRelease);
		parameters.addParameter(releaseParameter);
		auto outputParameter = new OutputParameter(ParameterInfo::kCanAutomate, ParameterIds::kOutput);
		parameters.addParameter(outputParameter);
		auto kneeParameter = new KneeParameter(ParameterInfo::kCanAutomate, ParameterIds::kKnee);
		parameters.addParameter(kneeParameter);
		auto reductionParameter = new ReductionParameter(ParameterInfo::kIsReadOnly, ParameterIds::kReduction);
		parameters.addParameter(reductionParameter);

		return kResultOk;
	}

	tresult PLUGIN_API AudioCompressorControllerSimple::setComponentState(IBStream* state) {
		for (int i = 0; i < ParameterIds::kNumParams; i++) {
			double parameterToLoad(0);
			if (state->read(&parameterToLoad, sizeof(double)) == kResultOk) {
				setParamNormalized(i, parameterToLoad);
			}
		}
		return kResultOk;
	}

}
}
}
