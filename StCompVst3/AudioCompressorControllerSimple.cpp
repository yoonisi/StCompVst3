
#include <math.h>
#include "AudioCompressorParameters.h"
#include "AudioCompressorControllerSimple.h"
#include "AudioCompressorEditor.h"
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
		this->viewArray.removeAll();
		return EditControllerEx1::terminate();
	}


	tresult PLUGIN_API AudioCompressorControllerSimple::setComponentState(IBStream* state) {

		LOG(Logger::kINFO, "");

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

	IPlugView* PLUGIN_API AudioCompressorControllerSimple::createView(const char * name) {
		if (name != 0 && strcmp(name, "editor") == 0) {
			auto view = new AudioCompressorEditor(this);
			return view;
		}
		else {
			return 0;
		}
	}

	tresult PLUGIN_API AudioCompressorControllerSimple::setParamNormalized(ParamID tag, ParamValue value) {
		auto result = EditControllerEx1::setParamNormalized(tag, value);
		for (int i = 0; i < this->viewArray.total(); i++) {
			if (viewArray.at(i)) {
				viewArray.at(i)->update(tag, value);
			}
		}
		return result;
	}

	tresult PLUGIN_API AudioCompressorControllerSimple::queryInterface(const char * iid, void ** obj)
	{
		return EditControllerEx1::queryInterface(iid, obj);
	}

	void AudioCompressorControllerSimple::addDependentView(AudioCompressorEditor* view) {
		this->viewArray.add(view);
	}

	void AudioCompressorControllerSimple::removeDependentView(AudioCompressorEditor* view) {
		for (int i = 0; i < this->viewArray.total(); i++) {
			if (this->viewArray.at(i) == view) {
				viewArray.removeAt(i);
				break;
			}
		}
	}

	void AudioCompressorControllerSimple::editorAttached(EditorView* editor) {
		auto view = dynamic_cast<AudioCompressorEditor*>(editor);
		if (view) {
			this->addDependentView(view);
		}
	}

	void AudioCompressorControllerSimple::editorRemoved(EditorView* editor) {
		auto view = dynamic_cast<AudioCompressorEditor*>(editor);
		if (view) {
			this->removeDependentView(view);
		}
	}

	tresult AudioCompressorControllerSimple::receiveText(const char * text) {
		if (text) {
			LOG(Logger::kINFO, text);
		}
		return kResultOk;
	}

}
}
}
