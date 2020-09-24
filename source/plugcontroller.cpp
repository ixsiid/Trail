#include "../include/plugcontroller.h"

#include <base/source/fstreamer.h>
#include <pluginterfaces/base/fplatform.h>
#include <pluginterfaces/base/ibstream.h>

#include "../include/gui.h"
#include "../include/initialize_message.h"
#include "../include/log.h"
#include "../include/plugids.h"

namespace Steinberg {
namespace HelloWorld {

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugController::initialize(FUnknown* context) {
	LOG("Controller Initialize");
	tresult result = EditController::initialize(context);
	if (result == kResultTrue) {
		//---Create Parameters------------
		parameters.addParameter(STR16("Midi CC out num"), 0, 0, 0,
						    Vst::ParameterInfo::kCanAutomate,
						    HelloWorldParams::kCCId,
						    0, STR16("MIDI CC"));

		parameters.addParameter(STR16("Noise Level"), 0, 0, 0.0,
						    Vst::ParameterInfo::kIsHidden | Vst::ParameterInfo::kIsReadOnly,
						    HelloWorldParams::kNoiseLevel,
						    0, STR16("Noise"));

		parameters.addParameter(STR16("F0"), STR16("Hz"), 0, 0.0,
						    Vst::ParameterInfo::kCanAutomate,
						    HelloWorldParams::kParamF0,
						    0, STR16("F0"));

		parameters.addParameter(STR16("Fp"), STR16("[-1, 1]"), 0, 0.0,
						    Vst::ParameterInfo::kCanAutomate,
						    HelloWorldParams::kParamFp,
						    0, STR16("Fp"));
	}
	
	LOG("... Done");
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState(IBStream* state) {
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	if (!state)
		return kResultFalse;

	return kResultOk;
}

IPlugView* PLUGIN_API PlugController::createView(FIDString name) {
	if (strcmp(name, "editor") == 0) {
		return nullptr;
		GUI* view = new GUI(this, dft, proj);
		return view;
	}

	return nullptr;
}

tresult PLUGIN_API PlugController::notify(Vst::IMessage* message) {
	if (strcmp(message->getMessageID(), u8"INITIALIZE") == 0) {
		int64 p;
		tresult r = message->getAttributes()->getInt(u8"PARAMETER", p);
		if (r == kResultOk) {
			InitializeMessage * init = (InitializeMessage *)p;
			dft = init->dft;
			proj = init->projection;
			
			this->setParamNormalized(kCCId, init->midi_cc_num);
			this->setParamNormalized(kNoiseLevel, init->noise_level);

			return kResultOk;
		}
	}

	return ComponentBase::notify(message);
}

//------------------------------------------------------------------------
}  // namespace HelloWorld
}  // namespace Steinberg
