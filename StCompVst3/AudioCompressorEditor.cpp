#include "AudioCompressorEditor.h"


namespace Steinberg {
namespace Vst {
namespace StComp
{

	AudioCompressorEditor::AudioCompressorEditor(void* controller) :
	 VSTGUIEditor(controller),
		background(0)
	{
		setIdleRate(50);
		ViewRect viewRect(0, 0, kEditorWidth, kEditorHeight);
		setRect(viewRect);
	}

	tresult PLUGIN_API AudioCompressorEditor::onSize(ViewRect* newSize) {
		auto result = VSTGUIEditor::onSize(newSize);
		return result;
	}



	AudioCompressorEditor::~AudioCompressorEditor(){}


}
}
}
