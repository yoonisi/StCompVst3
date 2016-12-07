#pragma once

#include <string.h>

#include "public.sdk/source/vst/vstguieditor.h"
#include "pluginterfaces/vst/ivstplugview.h"
#include "pluginterfaces/vst/ivstcontextmenu.h"

namespace Steinberg {
namespace Vst {
namespace StComp
{
	class AudioCompressorEditor :
		public VSTGUIEditor,
		public CControlListener,
		public IParameterFinder,
		public IContextMenuTarget 
	{
	public:
		AudioCompressorEditor(void* controller);
		virtual ~AudioCompressorEditor();

		//-- VSTGUIEditor
		bool PLUGIN_API open(void* parent);
		void PLUGIN_API close();
		CMessageResult notify(CBaseObject* sender, const char* message);
		void beginEdit(long index);
		void endEdit(long index);


		//-- CControlListener
		void valueChanged(CControl* pControl);
		long controlModifierClicked(CControl* pControl, long button);
		void controlBeginEdit(CControl* pControl);
		void controlEndEdit(CControl* pControl);

		//--EditorView
		tresult PLUGIN_API onSize(ViewRect* newSize);
		tresult PLUGIN_API canResize() { return kResultFalse; }
		tresult PLUGIN_API checkSizeConstraint(ViewRect* rect);

		//--IParameterFinder
		tresult PLUGIN_API findParameter(int32 xPos, int32 yPos, ParamID& resultTag);

		//--IContextMenuTarget
		tresult PLUGIN_API executeMenuItem(int32 tag);

		DELEGATE_REFCOUNT(VSTGUIEditor)
		tresult PLUGIN_API queryInterface(const char* iid, void** obj);

		void update(ParamID tag, ParamValue value);

	protected:
		CBitmap* background;
		double lastReductionMeterValue;

		CVuMeter* reductionMeter;

	};
}
}
}
