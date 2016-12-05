
#include <math.h>
#include "AudioCompressorParameters.h"
#include "AudioCompressorControllerSimple.h"
#include "logger.h"

namespace Steinberg {
namespace Vst {
namespace StComp {

	using namespace LogTool;

	tresult PLUGIN_API AudioCompressorControllerSimple::initialize(FUnknown* context) {
		tresult result = EditController::initialize(context);
		if (result != kResultOk) {
			return result;
		}

		UnitInfo unitInfo;
		Unit* unit;

		// create a unit1 for the gain
		unitInfo.id = 1;
		unitInfo.parentUnitId = kRootUnitId;	// attached to the root unit

		Steinberg::UString(unitInfo.name, USTRINGSIZE(unitInfo.name)).assign(USTRING("Unit1"));

		unitInfo.programListId = kNoProgramListId;

		unit = new Unit(unitInfo);
		addUnit(unit);

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

		for (int i = 0; i < parameters.getParameterCount(); i++) {
			parameters.getParameterByIndex(i)->setUnitID(1);
		}

		return kResultOk;
	}

	tresult PLUGIN_API AudioCompressorControllerSimple::terminate() {
		return EditController::terminate();
	}


	tresult PLUGIN_API AudioCompressorControllerSimple::setComponentState(IBStream* state) {

		LOG(Logger::INFO, "");

		for (int i = 0; i < ParameterIds::kNumParams; i++) {
			double parameterToLoad(0);
			if (state->read(&parameterToLoad, sizeof(double)) == kResultOk) {
#if BYTEORDER == kBigEndian
				SWAP_64(paramToLoad(0), sizeof(double));
#endif
				setParamNormalized(i, parameterToLoad);

			}
		}
		return kResultOk;
	}

}
}
}
