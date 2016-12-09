#include "AudioCompressorEditor.h"
#include "AudioCompressorConsts.h"
#include "AudioCompressorParameters.h"
#include "resource.h"
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

	enum UITag {
		kUIThreshold = 0,
		kUIThresholdText,
		kUIRatio,
		kUIRatioText,
		kUIAttack,
		kUIAttackText,
		kUIRelease,
		kUIReleaseText,
		kUIOutput,
		kUIOutputText,
		kUIKnee
	};

	AudioCompressorEditor::AudioCompressorEditor(void* controller) :
		VSTGUIEditor(controller),
		background(0),
		reductionMeter(0),
		thresholdKnob(0),
		ratioKnob(0),
		attackKnob(0),
		releaseKnob(0),
		outputKnob(0),
		kneeFader(0),
		thresholdText(0),
		ratioText(0),
		attackText(0),
		releaseText(0),
		outputText(0)

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

	tresult PLUGIN_API AudioCompressorEditor::findParameter(int32 xPos, int32 yPos, ParamID& resultTag) {
		CPoint where(xPos, yPos);
		if (this->thresholdKnob->hitTest(where)) {
			resultTag = ParameterIds::kThreshold;
			return kResultOk;
		}
		if (this->ratioKnob->hitTest(where)) {
			resultTag = ParameterIds::kRatio;
			return kResultOk;
		}
		if (this->attackKnob->hitTest(where)) {
			resultTag = ParameterIds::kAttack;
			return kResultOk;
		}
		if (this->releaseKnob->hitTest(where)) {
			resultTag = ParameterIds::kRelease;
			return kResultOk;
		}
		if (this->outputKnob->hitTest(where)) {
			resultTag = ParameterIds::kOutput;
			return kResultOk;
		}
		if (this->kneeFader->hitTest(where)) {
			resultTag = ParameterIds::kKnee;
			return kResultOk;
		}

		return kResultFalse;
	}

	tresult PLUGIN_API AudioCompressorEditor::queryInterface(const char* iid, void** obj) {
		QUERY_INTERFACE(iid, obj, IParameterFinder::iid, IParameterFinder);
		QUERY_INTERFACE(iid, obj, IContextMenuTarget::iid, IContextMenuTarget);
		return VSTGUIEditor::queryInterface(iid, obj);
	}

	static void configParamDisplay(CParamDisplay* text, CColor& fontColor, CColor& bgColor) {
		text->setFont(kNormalFontSmall);
		text->setFontColor(fontColor);
		text->setBackColor(bgColor);
		text->setFrameColor(bgColor);
	}

	static void positonParamDisplay(CRect& size) {

		size.left -= 10;
		size.right += 10;
		size.top = kKnobDispY;
		size.bottom = kKnobDispBottom;

	}

	void DbStringConvert(float value, char* string)
	{
		sprintf(string, "%.1fdB", -60.0f * (1.0f - value));
	}

	bool AudioCompressorEditor::open(void* parent) {
		if (frame) {
			return false;
		}

		{
			// create background image
			CRect editorSize(0, 0, kEditorWidth, kEditorHeight);
			frame = new CFrame(editorSize, parent, this);
			frame->setBackgroundColor(kGreyCColor);
			background = new CBitmap(IDB_BACK);
			frame->setBackground(this->background);
		}

		{
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
		}


		auto knobBitMap = new CBitmap(IDB_KNOB);
		CColor fontColor;
		fontColor.alpha = 0xff;
		fontColor.red = fontColor.blue = fontColor.green = 0xff;
		CColor bgColor;
		bgColor.alpha = 0xff;
		bgColor.red = bgColor.blue = bgColor.green = 51;
		
		{
			// create threshold knob
			CRect size(0, 0, kKnobSize, kKnobSize);
			size.offset(kKnobX, kKnobY);
			this->thresholdKnob = new CAnimKnob(
				size,
				this,
				UITag::kUIThreshold,
				kKnobNum,
				kKnobSize,
				knobBitMap
			);
			this->thresholdKnob->setMin(0.f);
			this->thresholdKnob->setMax(1.f);
			this->frame->addView(this->thresholdKnob);
			auto value = getController()->getParamNormalized(ParameterIds::kThreshold);
			
			positonParamDisplay(size);
			this->thresholdText = new CParamDisplay(size);
			configParamDisplay(this->thresholdText, fontColor, bgColor);
			this->thresholdText->setStringConvert(ThresholdParameter::stringConvert);
			this->frame->addView(this->thresholdText);

			update(kThreshold, value);
		}

		{
			// create ratio knob
			CRect size(0, 0, kKnobSize, kKnobSize);
			size.offset(kKnobX + kKnobInc, kKnobY);
			this->ratioKnob = new CAnimKnob(
				size,
				this,
				UITag::kUIRatio,
				kKnobNum,
				kKnobSize,
				knobBitMap
			);
			this->ratioKnob->setMin(0.f);
			this->ratioKnob->setMax(1.f);
			this->frame->addView(this->ratioKnob);
			auto value = getController()->getParamNormalized(ParameterIds::kRatio);

			positonParamDisplay(size);
			this->ratioText = new CParamDisplay(size);
			configParamDisplay(this->ratioText, fontColor, bgColor);
			this->ratioText->setStringConvert(RatioParameter::stringConvert);
			this->frame->addView(this->ratioText);

			update(kRatio, value);
		}
		{
			// create attack knob
			CRect size(0, 0, kKnobSize, kKnobSize);
			size.offset(kKnobX + 2 * kKnobInc, kKnobY);
			this->attackKnob = new CAnimKnob(
				size,
				this,
				UITag::kUIAttack,
				kKnobNum,
				kKnobSize,
				knobBitMap
			);
			this->attackKnob->setMin(0.f);
			this->attackKnob->setMax(1.f);
			this->frame->addView(this->attackKnob);
			auto value = getController()->getParamNormalized(ParameterIds::kAttack);

			positonParamDisplay(size);
			this->attackText = new CParamDisplay(size);
			configParamDisplay(this->attackText, fontColor, bgColor);
			this->attackText->setStringConvert(AttackParameter::stringConvert);
			this->frame->addView(this->attackText);

			update(kAttack, value);
		}
		{
			// create release knob
			CRect size(0, 0, kKnobSize, kKnobSize);
			size.offset(kKnobX + 3 * kKnobInc, kKnobY);
			this->releaseKnob = new CAnimKnob(
				size,
				this,
				UITag::kUIRelease,
				kKnobNum,
				kKnobSize,
				knobBitMap
			);
			this->releaseKnob->setMin(0.f);
			this->releaseKnob->setMax(1.f);
			this->frame->addView(this->releaseKnob);
			auto value = getController()->getParamNormalized(ParameterIds::kRelease);

			positonParamDisplay(size);
			this->releaseText = new CParamDisplay(size);
			configParamDisplay(this->releaseText, fontColor, bgColor);
			this->releaseText->setStringConvert(ReleaseParameter::stringConvert);
			this->frame->addView(this->releaseText);

			update(kRelease, value);
		}
		{
			// create gain knob
			CRect size(0, 0, kKnobSize, kKnobSize);
			size.offset(kKnobX + 4 * kKnobInc, kKnobY);
			this->outputKnob = new CAnimKnob(
				size,
				this,
				UITag::kUIOutput,
				kKnobNum,
				kKnobSize,
				knobBitMap
			);
			this->outputKnob->setMin(0.f);
			this->outputKnob->setMax(1.f);
			this->frame->addView(this->outputKnob);
			auto value = getController()->getParamNormalized(ParameterIds::kOutput);

			positonParamDisplay(size);
			this->outputText = new CParamDisplay(size);
			configParamDisplay(this->outputText, fontColor, bgColor);
			this->outputText->setStringConvert(OutputParameter::stringConvert);
			this->frame->addView(this->outputText);

			update(kOutput, value);
		}
		knobBitMap->forget();

		{
			// create knee fader
			CRect size(0, 0, kKneeSizeX, kKneeSizeY);
			size.offset(kKneeX, kKneeY);
			CBitmap* faderBodyBmp = new CBitmap(IDB_FADERBODY);
			CBitmap* faderHandleBmp = new CBitmap(IDB_FADERHANDLE);
			int minPosition = kKneeX;
			int maxPosition = kKneeX + kKneeSizeX - faderHandleBmp->getWidth();
			this->kneeFader = new CHorizontalSlider(
				size,
				this,
				kUIKnee,
				minPosition,
				maxPosition,
				faderHandleBmp,
				faderBodyBmp
			);
			this->kneeFader->setMin(0.f);
			this->kneeFader->setMax(1.f);
			this->frame->addView(this->kneeFader);
			auto value = getController()->getParamNormalized(ParameterIds::kKnee);
			update(kKnee, value);
			faderBodyBmp->forget();
			faderHandleBmp->forget();
		}

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
		this->reductionMeter = 0;
		this->thresholdKnob = 0;
		this->ratioKnob = 0;
		this->attackKnob = 0;
		this->releaseKnob = 0;
		this->outputKnob = 0;
	}

	void AudioCompressorEditor::valueChanged(CControl* pControl) {
		auto uitag = pControl->getTag();
		switch (uitag)
		{
		case UITag::kUIThreshold:
			controller->setParamNormalized(ParameterIds::kThreshold, pControl->getValue());
			controller->performEdit(ParameterIds::kThreshold, pControl->getValue());
			break;
		case UITag::kUIRatio:
			controller->setParamNormalized(ParameterIds::kRatio, pControl->getValue());
			controller->performEdit(ParameterIds::kRatio, pControl->getValue());
			break;
		case UITag::kUIAttack:
			controller->setParamNormalized(ParameterIds::kAttack, pControl->getValue());
			controller->performEdit(ParameterIds::kAttack, pControl->getValue());
			break;
		case UITag::kUIRelease:
			controller->setParamNormalized(ParameterIds::kRelease, pControl->getValue());
			controller->performEdit(ParameterIds::kRelease, pControl->getValue());
			break;
		case UITag::kUIOutput:
			controller->setParamNormalized(ParameterIds::kOutput, pControl->getValue());
			controller->performEdit(ParameterIds::kOutput, pControl->getValue());
			break;
		case UITag::kUIKnee:
			controller->setParamNormalized(ParameterIds::kKnee, pControl->getValue());
			controller->performEdit(ParameterIds::kKnee, pControl->getValue());
			break;
		default:
			break;
		}
	}

	tresult PLUGIN_API AudioCompressorEditor::executeMenuItem(int32 tag) {
		return kResultFalse;
	}

	long AudioCompressorEditor::controlModifierClicked(CControl* pControl, long button) {
		return 0;
	}

	void AudioCompressorEditor::controlBeginEdit(CControl* pControl) {
		auto uitag = pControl->getTag();
		switch (uitag)
		{

		case UITag::kUIThreshold:
			controller->beginEdit(ParameterIds::kThreshold);
			break;
		case UITag::kUIRatio:
			controller->beginEdit(ParameterIds::kRatio);
			break;
		case UITag::kUIAttack:
			controller->beginEdit(ParameterIds::kAttack);
			break;
		case UITag::kUIRelease:
			controller->beginEdit(ParameterIds::kRelease);
			break;
		case UITag::kUIOutput:
			controller->beginEdit(ParameterIds::kOutput);
			break;
		case UITag::kUIKnee:
			controller->beginEdit(ParameterIds::kKnee);
			break;
			
		default:
			break;
		}
	}

	void AudioCompressorEditor::controlEndEdit(CControl* pControl) {
		auto uitag = pControl->getTag();
		switch (uitag)
		{

		case UITag::kUIThreshold:
			controller->endEdit(ParameterIds::kThreshold);
			break;
		case ParameterIds::kRatio:
			controller->endEdit(ParameterIds::kRatio);
			break;
		case ParameterIds::kAttack:
			controller->endEdit(ParameterIds::kAttack);
			break;
		case ParameterIds::kRelease:
			controller->endEdit(ParameterIds::kRelease);
			break;
		case ParameterIds::kOutput:
			controller->endEdit(ParameterIds::kOutput);
			break;
		case ParameterIds::kKnee:
			controller->endEdit(ParameterIds::kKnee);
			break;

		default:
			break;
		}
	}

	void AudioCompressorEditor::beginEdit(long index){}
	void AudioCompressorEditor::endEdit(long index) {}

	void AudioCompressorEditor::update(ParamID tag, ParamValue value) {
		using namespace LogTool;
		switch (tag)
		{

		case ParameterIds::kThreshold:
			if (this->thresholdKnob) this->thresholdKnob->setValue(value);
			if (this->thresholdText) this->thresholdText->setValue(value);
			break;
		case ParameterIds::kRatio:
			if (this->ratioKnob) this->ratioKnob->setValue(value);
			if (this->ratioText) this->ratioText->setValue(value);
			break;
		case ParameterIds::kAttack:
			if (this->attackKnob) this->attackKnob->setValue(value);
			if (this->attackText) this->attackText->setValue(value);
			break;
		case ParameterIds::kRelease:
			if (this->releaseKnob) this->releaseKnob->setValue(value);
			if (this->releaseText) this->releaseText->setValue(value);
			break;
		case ParameterIds::kOutput:
			if (this->outputKnob) this->outputKnob->setValue(value);
			if (this->outputText) this->outputText->setValue(value);
			break;
		case ParameterIds::kKnee:
			if (this->kneeFader) this->kneeFader->setValue(value);
			break;

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
