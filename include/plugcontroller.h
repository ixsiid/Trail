#pragma once

#include <public.sdk/source/vst/vsteditcontroller.h>
#include <vstgui/plugin-bindings/vst3editor.h>

#include "dft.h"
#include "projection.h"

namespace Steinberg {
namespace HelloWorld {

class PlugController : public Vst::EditController {
    public:
	static FUnknown* createInstance(void*) {
		return (Vst::IEditController*)new PlugController();
	}

	tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
	tresult PLUGIN_API setComponentState(IBStream* state) SMTG_OVERRIDE;
	IPlugView* PLUGIN_API createView(FIDString name);

	tresult PLUGIN_API notify(Vst::IMessage* message) SMTG_OVERRIDE;

    private:
	DFT * dft;
	Projection * proj;
};

}  // namespace HelloWorld
}  // namespace Steinberg
