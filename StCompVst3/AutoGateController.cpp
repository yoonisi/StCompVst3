#include "AutoGateController.h"

namespace Steinberg {
namespace Vst {
namespace StGate {

tresult PLUGIN_API StGate::AutoGateController::initialize(FUnknown * context)
{
	tresult result = EditControllerEx1::initialize(context);
	if (result != kResultOk) {
		return result;
	}

}


}
}
}
