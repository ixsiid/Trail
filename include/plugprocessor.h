#pragma once

#include <public.sdk/source/vst/vstaudioeffect.h>

#include "dft.h"
#include "projection.h"

namespace Steinberg {
namespace HelloWorld {

//-----------------------------------------------------------------------------
class PlugProcessor : public Vst::AudioEffect {
    public:
	PlugProcessor();
	~PlugProcessor();

	tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API setBusArrangements(Vst::SpeakerArrangement* inputs, int32 numIns,
								   Vst::SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;

	tresult PLUGIN_API setupProcessing(Vst::ProcessSetup& setup) SMTG_OVERRIDE;
	tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
	tresult PLUGIN_API process(Vst::ProcessData& data) SMTG_OVERRIDE;

	//------------------------------------------------------------------------
	tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
	tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

	static FUnknown* createInstance(void*) {
		return (Vst::IAudioProcessor*)new PlugProcessor();
	}

    private:
	float* buffer;
	int index;

	int cc = 0;

	const int dftnum = 8192;
	DFT* dft;
	Projection* proj;

	const int version = 100;
};

//------------------------------------------------------------------------
}  // namespace HelloWorld
}  // namespace Steinberg
