#include "../include/plugprocessor.h"

#include <base/source/fstreamer.h>
#include <pluginterfaces/base/ibstream.h>
#include <pluginterfaces/vst/ivstevents.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>

#include "../include/dft.h"
#include "../include/gui.h"
#include "../include/initialize_message.h"
#include "../include/log.h"
#include "../include/plugids.h"
#include "../include/waveview.h"

namespace Steinberg {
namespace HelloWorld {

//-----------------------------------------------------------------------------
PlugProcessor::PlugProcessor() {
	LOG("Processor constructor");

	// register its editor class
	setControllerClass(MyControllerUID);

	int bufferSize = dftnum * 2 + 4096;

	buffer = new float[bufferSize];
	for (int i = 0; i < bufferSize; i++) buffer[i] = 0;
	index = 0;

	dft	= new DFT(dftnum, 44100.0);
	proj = new Projection();

	LOG("... Done");
}

PlugProcessor::~PlugProcessor() {
	delete[] buffer;
	delete dft;
	delete proj;
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::initialize(FUnknown* context) {
	LOG("Processor Initialize");
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize(context);
	if (result != kResultTrue) return kResultFalse;

	//---create Audio In/Out busses------
	// we want a stereo Input and a Stereo Output
	addAudioInput(STR16("AudioInput"), Vst::SpeakerArr::kMono);
	addAudioOutput(STR16("AudioOutput"), Vst::SpeakerArr::kMono);

	addEventOutput(STR16("MIDI Out"), 1);

	LOG("... Done");
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
	LOGN("Set sample rate: %f", setup.sampleRate);
	return dft->setSampleRate(setup.sampleRate) | AudioEffect::setupProcessing(setup);
}

//-----------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setActive(TBool state) {
	if (state) {  // Initialize
		// Allocate Memory Here
		InitializeMessage initializeMessage;
		initializeMessage.dft	    = dft;
		initializeMessage.projection = proj;

		initializeMessage.midi_cc_num = cc / 119.0f;
		initializeMessage.noise_level = (dft->noise_level / 32.0f) + ((256.0f - 160.0f) / 256.0f);

		Vst::IMessage* message = allocateMessage();
		message->setMessageID(u8"INITIALIZE");
		message->getAttributes()->setInt(u8"PARAMETER", (int64)&initializeMessage);
		sendMessage(message);
	} else {	// Release
			// Free Memory if still allocated
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
					case HelloWorldParams::kCCId:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							cc = value * 119;
						}
						break;
					case HelloWorldParams::kNoiseLevel:
						if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
							dft->noise_level = (value - ((256.0 - 160.0) / 256.0)) * 32.0;
						}
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

		if (data.outputEvents) data.outputEvents->addEvent(event);

		if (data.outputParameterChanges) {
			value /= 512.0;
			value = 1.0 - value;
			if (value < 0.0)
				value = 0.0;
			else if (value > 1.0)
				value = 1.0;

			int32 paramIndex = 0;
			int32 queueIndex = 0;
			tresult r;

			r = data.outputParameterChanges
				   ->addParameterData(HelloWorldParams::kParamFp, paramIndex)
				   ->addPoint(0, value, queueIndex);

			r = data.outputParameterChanges
				   ->addParameterData(HelloWorldParams::kParamF0, paramIndex)
				   ->addPoint(0, dft->f0 * 44100.0 / 8192.0 / 1400.0 / 2.0, queueIndex);
		}
	}
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::setState(IBStream* state) {
	if (!state) return kResultFalse;
	// called when we load a preset or project, the model has to be reloaded
	IBStreamer streamer(state, kLittleEndian);

	uint32 _v;
	if (!streamer.readInt32u(_v)) return kResultFalse;
	if (_v != version) return kResultFalse;

	if (!proj->load(&streamer)) return kResultFalse;

	int32 _cc;
	if (!streamer.readInt32(_cc)) return kResultFalse;
	cc = _cc;

	float _noise_level;
	if (!streamer.readFloat(_noise_level)) return kResultFalse;
	dft->noise_level = _noise_level;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API PlugProcessor::getState(IBStream* state) {
	// here we need to save the model (preset or project)
	IBStreamer streamer(state, kLittleEndian);

	streamer.writeInt32u(version);

	proj->save(&streamer);

	streamer.writeInt32(cc);
	streamer.writeFloat(dft->noise_level);

	return kResultOk;
}

//------------------------------------------------------------------------
}  // namespace HelloWorld
}  // namespace Steinberg
