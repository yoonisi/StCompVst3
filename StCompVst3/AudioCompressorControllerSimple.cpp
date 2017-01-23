
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
		tresult result = EditControllerEx1::initialize(context);
		if (result != kResultOk) {
			return result;
		}

		addUnit(new Unit(String("Root"), kRootUnitId, kNoParentUnitId));

		UnitID uid = kRootUnitId;

		auto thresholdParameter = new ThresholdParameter(ParameterInfo::kCanAutomate, ParameterIds::kThreshold, uid);
		parameters.addParameter(thresholdParameter);
		auto ratioParameter = new RatioParameter(ParameterInfo::kCanAutomate, ParameterIds::kRatio, uid);
		parameters.addParameter(ratioParameter);
		auto attackParameter = new AttackParameter(ParameterInfo::kCanAutomate, ParameterIds::kAttack, uid);
		parameters.addParameter(attackParameter);
		auto releaseParameter = new ReleaseParameter(ParameterInfo::kCanAutomate, ParameterIds::kRelease, uid);
		parameters.addParameter(releaseParameter);
		auto outputParameter = new OutputParameter(ParameterInfo::kCanAutomate, ParameterIds::kOutput, uid);
		parameters.addParameter(outputParameter);
		auto kneeParameter = new KneeParameter(ParameterInfo::kCanAutomate, ParameterIds::kKnee, uid);
		parameters.addParameter(kneeParameter);
		auto reductionParameter = new ReductionParameter(ParameterInfo::kIsReadOnly, ParameterIds::kReduction, uid);
		parameters.addParameter(reductionParameter);
		auto bypassParameter = new BypassParameter(ParameterInfo::kCanAutomate | ParameterInfo::kIsBypass, ParameterIds::kBypassProcess, uid);
		parameters.addParameter(bypassParameter);

		ProgramList* programList = new ProgramList(String("Preset"), 'prg', kRootUnitId);
		this->addProgramList(programList);
		programList->addProgram(String("default"));
		parameters.addParameter(programList->getParameter());
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
		LOG(Logger::kINFO, name);
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
