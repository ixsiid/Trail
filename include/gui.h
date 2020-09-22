#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "dft.h"
#include "projection.h"
#include "numericChangeView.h"

#include "projectionView.h"
#include "waveview.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class GUI : public Vst::VSTGUIEditor, public IControlListener {
    public:
	GUI(void* controller, DFT* dft, Projection * proj);

	// VSTGUIEditor
	virtual bool PLUGIN_API open(void* parent, const PlatformType& PlatformType = kDefaultNative);
	virtual void PLUGIN_API close();

	CMessageResult notify(CBaseObject* sender, const char* message);

	// IControlListener
	void valueChanged(CControl* pControl);

	DELEGATE_REFCOUNT(VSTGUIEditor);

    private:
	WaveView* wave;
	ProjectionView* projection;

	DFT* dft;
	Projection* proj;
	NumericChangeView * ccView;
};

}  // namespace HelloWorld
}  // namespace Steinberg
