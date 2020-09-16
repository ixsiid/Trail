#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "waveview.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class GUI : public Vst::VSTGUIEditor, public IControlListener {
    public:
	GUI(void* controller);
	virtual bool PLUGIN_API open(void* parent, const PlatformType& PlatformType = kDefaultNative);
	virtual void PLUGIN_API close();
	void valueChanged(CControl* pControl);
//	CMessageResult notify(CBaseObject* sender, const char* message);

	DELEGATE_REFCOUNT(GUI);

    private:
	CControl* createWaveView(int x, int y);
	WaveView* wave;
};

}  // namespace HelloWorld
}  // namespace Steinberg
