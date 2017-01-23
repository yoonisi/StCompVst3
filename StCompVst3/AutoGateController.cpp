#include "AutoGateController.h"
#include "AutoGateParameters.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

tresult PLUGIN_API StGate::AutoGateController::initialize(FUnknown * context)
{
	tresult result = EditControllerEx1::initialize(context);
	if (result != kResultOk) {
		return result;
	}

	addUnit(new Unit(String("Root"), kRootUnitId, kNoParentUnitId));

	UnitID uid = kRootUnitId;

	auto thresholdParameter = new ThresholdParameter(ParameterIds::kThreshold, uid);
	parameters.addParameter(thresholdParameter);
	auto attackParameter = new AttackParameter(ParameterIds::kAttack, uid);
	parameters.addParameter(attackParameter);
	auto holdParameter = new HoldParameter(ParameterIds::kHold, uid);
	parameters.addParameter(holdParameter);
	auto releaseParameter = new ReleaseParameter(ParameterIds::kRelease, uid);
	parameters.addParameter(releaseParameter);
	auto keyListenParameter = new KeyListendParameter(ParameterIds::kKeyListen, uid);
	parameters.addParameter(keyListenParameter);
	auto duckerParameter = new DuckerParameter(ParameterIds::kDucker, uid);
	parameters.addParameter(duckerParameter);
	auto keyHpfCutoffParameter = new HpfCutoffParameter(ParameterIds::kKeyHpfCutoff, uid);
	parameters.addParameter(keyHpfCutoffParameter);
	auto keyLpfCutoffParameter = new LpfCutoffParameter(ParameterIds::kKeyLpfCutoff, uid);
	parameters.addParameter(keyLpfCutoffParameter);
	auto bypassParameter = new BypassParameter(ParameterIds::kBypassProcess, uid);
	parameters.addParameter(bypassParameter);

	ProgramList* programList = new ProgramList(String("Preset"), 'prg', kRootUnitId);
	this->addProgramList(programList);
	programList->addProgram(String("default"));
	parameters.addParameter(programList->getParameter());

	return kResultOk;


}

tresult PLUGIN_API AutoGateController::terminate()
{
	return EditControllerEx1::terminate();
}

tresult PLUGIN_API AutoGateController::setComponentState(IBStream * state)
{
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
