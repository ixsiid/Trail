#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "waveview.h"
#include "projection.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class GUI : public Vst::VSTGUIEditor , public IControlListener {
    public:
	GUI(void* controller);

// VSTGUIEditor
	virtual bool PLUGIN_API open(void* parent, const PlatformType& PlatformType = kDefaultNative);
	virtual void PLUGIN_API close();

	CMessageResult notify(CBaseObject* sender, const char* message);

// IControlListener
	void valueChanged(CControl* pControl);

	DELEGATE_REFCOUNT(VSTGUIEditor);

    private:
	WaveView * wave;
	Projection * projection;
};

}  // namespace HelloWorld
}  // namespace Steinberg
