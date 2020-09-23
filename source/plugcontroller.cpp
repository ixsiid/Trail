//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugcontroller.cpp
// Created by  : Steinberg, 01/2018
// Description : HelloWorld Example for VST 3
//
//-----------------------------------------------------------------------------
// LICENSE
// (c) 2020, Steinberg Media Technologies GmbH, All Rights Reserved
//-----------------------------------------------------------------------------
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of the Steinberg Media Technologies nor the names of its
//     contributors may be used to endorse or promote products derived from this
//     software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

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
	tresult result = EditController::initialize(context);
	if (result == kResultTrue) {
		//---Create Parameters------------
		parameters.addParameter(STR16("Midi CC out num"), 0, 0, 0,
						    Vst::ParameterInfo::kCanAutomate,
						    HelloWorldParams::kCCId,
						    0, STR16("MIDI CC"));

		parameters.addParameter(STR16("Noise Level"), 0, 0, 0.0,
						    Vst::ParameterInfo::kIsHidden,
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
	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugController::setComponentState(IBStream* state) {
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	if (!state)
		return kResultFalse;

	LOG("set component state\n");

	return kResultOk;
}

IPlugView* PLUGIN_API PlugController::createView(FIDString name) {
	if (strcmp(name, "editor") == 0) {
		GUI* view = new GUI(this, dft, proj);
		return view;
	}

	return nullptr;
}

tresult PlugController::notify(Vst::IMessage* message) {
	LOG("RECV MESSAGE\n");
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
