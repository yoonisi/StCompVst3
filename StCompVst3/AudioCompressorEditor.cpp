#include "AudioCompressorEditor.h"
#include "resource.h"
#include "AudioCompressorConsts.h"
#include "logger.h"

namespace Steinberg {
namespace Vst {
namespace StComp
{
	enum UIPosition {

		kKnobX = 30,
		kKnobY = 70,

		kKnobSize = 30,
		kKnobInc = 50,
		kKnobNum = 25,

		kKnobDispY = 55,
		kKnobDispBottom = 65,

		kReductionX = 160,
		kReductionY = 30,
		kReductionSizeX = 110,
		kReductionSizeY = 5,

		kKneeX = 100,
		kKneeY = 130,
		kKneeSizeX = 130,
		kKneeSizeY = 10
	};

	enum UISize {
		kEditorWidth = 290,
		kEditorHeight = 170
	};
	AudioCompressorEditor::AudioCompressorEditor(void* controller) :
		VSTGUIEditor(controller),
		background(0),
		reductionMeter(0)
	{
		setIdleRate(50);
		ViewRect viewRect(0, 0, kEditorWidth, kEditorHeight);
		setRect(viewRect);
	}

	tresult PLUGIN_API AudioCompressorEditor::onSize(ViewRect* newSize) {
		auto result = VSTGUIEditor::onSize(newSize);
		return result;
	}

	tresult PLUGIN_API AudioCompressorEditor::checkSizeConstraint(ViewRect* rect) {
		// check right
		if (rect->right - rect->left < kEditorWidth) {
			rect->right = rect->left + kEditorWidth;
		}
		else if (rect->right - rect->left > kEditorWidth + 50) {
			rect->right = rect->left + kEditorWidth + 50;
		}
		if (rect->bottom - rect->top < kEditorHeight) {
			rect->bottom = rect->top + kEditorHeight;
		}
		else if (rect->bottom - rect->top < kEditorHeight + 50) {
			rect->bottom = rect->top + kEditorHeight + 50;
		}
		return kResultTrue;
	}

	tresult PLUGIN_API AudioCompressorEditor::findParameter(int32 xPos, int32 yPos, ParamID& result) {
		CPoint where(xPos, yPos);
		return kResultFalse;
	}

	tresult PLUGIN_API AudioCompressorEditor::queryInterface(const char* iid, void** obj) {
		QUERY_INTERFACE(iid, obj, IParameterFinder::iid, IParameterFinder);
		QUERY_INTERFACE(iid, obj, IContextMenuTarget::iid, IContextMenuTarget);
		return VSTGUIEditor::queryInterface(iid, obj);
	}

	bool AudioCompressorEditor::open(void* parent) {
		if (frame) {
			return false;
		}

		// create background image
		CRect editorSize(0, 0, kEditorWidth, kEditorHeight);
		frame = new CFrame(editorSize, parent, this);
		frame->setBackgroundColor(kGreyCColor);
		background = new CBitmap(IDB_BACK);
		frame->setBackground(this->background);
		
		// create reduction meter
		auto reductionOnLed = new CBitmap(IDB_LEDON);
		auto reductionOffLed = new CBitmap(IDB_LEDOFF);
		
		CRect size(0, 0, kReductionSizeX, kReductionSizeX);
		size.offset(kReductionX, kReductionY);
		this->reductionMeter = new CVuMeter(size, reductionOnLed, reductionOffLed, 44, kHorizontal);
		this->frame->addView(reductionMeter);
		this->reductionMeter->setDecreaseStepValue(0.02f);
		reductionOffLed->forget();
		reductionOnLed->forget();


		return true;
	}

	void PLUGIN_API AudioCompressorEditor::close() {
		if (frame) {
			delete frame;
			frame = 0;
		}
		if (background) {
			background->forget();
			background = 0;
		}
	}

	void AudioCompressorEditor::valueChanged(CControl* pControl) {

	}

	tresult PLUGIN_API AudioCompressorEditor::executeMenuItem(int32 tag) {
		return kResultFalse;
	}

	long AudioCompressorEditor::controlModifierClicked(CControl* pControl, long button) {
		return 0;
	}

	void AudioCompressorEditor::controlBeginEdit(CControl* pControl) {

	}

	void AudioCompressorEditor::controlEndEdit(CControl* pControl) {

	}

	void AudioCompressorEditor::beginEdit(long index){}
	void AudioCompressorEditor::endEdit(long index) {}

	void AudioCompressorEditor::update(ParamID tag, ParamValue value) {
		using namespace LogTool;
		switch (tag)
		{
		case ParameterIds::kReduction:
			this->lastReductionMeterValue = value;
			break;
		default:
			LOG(Logger::kERROR, "unknown index");
			break;
		}
	}

	CMessageResult AudioCompressorEditor::notify(CBaseObject* sender, const char* messaage) {
		if (messaage == CVSTGUITimer::kMsgTimer) {
			if (this->reductionMeter) {
				this->reductionMeter->setValue(this->lastReductionMeterValue);
				this->lastReductionMeterValue = 0;
			}
		}
		return VSTGUIEditor::notify(sender, messaage);
	}

	AudioCompressorEditor::~AudioCompressorEditor(){}


}
}
}
