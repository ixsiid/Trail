//------------------------------------------------------------------------
// Project     : VST SDK
//
// Category    : Examples
// Filename    : plugprocessor.cpp
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

#include "../include/plugprocessor.h"

#include <base/source/fstreamer.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstevents.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>

#include "../include/dft.h"
#include "../include/gui.h"
#include "../include/plugids.h"
#include "../include/waveview.h"

#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {

//-----------------------------------------------------------------------------
PlugProcessor::PlugProcessor() {
	new Log();

	// register its editor class
	setControllerClass(MyControllerUID);

	int bufferSize = dftnum * 2 + 4096;

	buffer = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) buffer[i] = 0;
	index = 0;

	dft	= new DFT(dftnum);
	proj = new Projection();
}

PlugProcessor::~PlugProcessor() {
	delete [] buffer;
	delete dft;
	delete proj;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::initialize(FUnknown* context) {
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize(context);
	if (result != kResultTrue) return kResultFalse;

	//---create Audio In/Out busses------
	// we want a stereo Input and a Stereo Output
	addAudioInput(STR16("AudioInput"), Vst::SpeakerArr::kMono);
	addAudioOutput(STR16("AudioOutput"), Vst::SpeakerArr::kMono);

	addEventOutput(STR16("MIDI Out"), 1);

	return kResultTrue;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setBusArrangements(Vst::SpeakerArrangement* inputs,
										   int32 numIns,
										   Vst::SpeakerArrangement* outputs,
										   int32 numOuts) {
	// we only support one in and output bus and these busses must have the same number of channels
	if (numIns == 1 && numOuts == 1 && inputs[0] == outputs[0]) {
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}
	return kResultFalse;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setupProcessing(Vst::ProcessSetup& setup) {
	// here you get, with setup, information about:
	// sampleRate, processMode, maximum number of samples per audio block
	return AudioEffect::setupProcessing(setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setActive(TBool state) {
	if (state) {  // Initialize
		// Allocate Memory Here
		LOG("set active\n");

		Vst::IMessage* messageA = allocateMessage();
		messageA->setMessageID(u8"INITIALIZE");
		messageA->getAttributes()->setInt(u8"DFT", (int64)dft);
		sendMessage(messageA);

		Vst::IMessage* messageB = allocateMessage();
		messageB->setMessageID(u8"INITIALIZE");
		messageB->getAttributes()->setInt(u8"PROJECTION", (int64)proj);
		sendMessage(messageB);
	} else {	// Release
		// Free Memory if still allocated

		LOG("set deactive\n");
	}

	return AudioEffect::setActive(state);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::process(Vst::ProcessData& data) {
	//--- Read inputs parameter changes-----------
	if (data.inputParameterChanges) {
		int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
		for (int32 index = 0; index < numParamsChanged; index++) {
			Vst::IParamValueQueue* paramQueue = data.inputParameterChanges->getParameterData(index);
			if (paramQueue) {
				Vst::ParamValue value;
				int32 sampleOffset;
				int32 numPoints = paramQueue->getPointCount();
				switch (paramQueue->getParameterId()) {
					case HelloWorldParams::kCCUpId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							cc++;
						}
						this->changed();
						break;
				}
			}
		}
	}

	//--- Process Audio---------------------
	//--- ----------------------------------
	if (data.numInputs == 0 || data.numOutputs == 0) {
		// nothing to do
		return kResultOk;
	}

	if (data.numSamples > 0) {
		int bus = 0;
		int ch  = 0;

		size_t size = sizeof(Vst::Sample32) * data.numSamples;

		// copy to output
		memcpy(data.outputs[bus].channelBuffers32[ch], data.inputs[bus].channelBuffers32[ch], size);

		// analyze f0
		memcpy(buffer + index, data.inputs[bus].channelBuffers32[ch], size);
		memcpy(buffer + index + dftnum, data.inputs[bus].channelBuffers32[ch], size);
		index = (index + data.numSamples) & (dftnum - 1);

		dft->process(buffer + index);
		double value = proj->evaluate(dft->f0);

		// output f0 signal
		int intValue = value / 4;
		if (intValue < 0)
			intValue = 0;
		else if (intValue > 127)
			intValue = 127;

		Vst::Event event			= {0};
		event.busIndex				= 0;
		event.sampleOffset			= 0;
		event.flags				= 1;
		event.type				= Vst::Event::kLegacyMIDICCOutEvent;
		event.midiCCOut.channel		= 0;
		event.midiCCOut.controlNumber = cc;
		event.midiCCOut.value		= 127 - intValue;
		event.midiCCOut.value2		= 0;

		data.outputEvents->addEvent(event);

		value /= 512.0;
		if (value < 0.0)
			value = 0.0;
		else if (value > 1.0)
			value = 1.0;

		int32 paramIndex = 0;
		int32 queueIndex = 0;

		data.outputParameterChanges
		    ->addParameterData(HelloWorldParams::kParamF0, paramIndex)
		    ->addPoint(0, 1.0 - value, queueIndex);
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setState(IBStream* state) {
	if (!state) return kResultFalse;
	// called when we load a preset or project, the model has to be reloaded
	
	LOG("set state\n");

	IBStreamer streamer(state, kLittleEndian);
	LOG("0\n");

	uint32 _v;
	if (!streamer.readInt32u(_v)) {
		LOG("1 fail\n");
		return kResultFalse;
	}
	
	LOG("1\n");
	if (_v != version) return kResultFalse;
	LOG("2\n");

	if (!proj->load(&streamer)) return kResultFalse;
	LOG("3\n");

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::getState(IBStream* state) {
	// here we need to save the model (preset or project)
	
	LOG("get state\n");

	IBStreamer streamer(state, kLittleEndian);

	streamer.writeInt32u(version);

	proj->save(&streamer);

	return kResultOk;
}

//------------------------------------------------------------------------
}  // namespace HelloWorld
}  // namespace Steinberg
