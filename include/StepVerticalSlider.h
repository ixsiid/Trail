#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class StepVerticalSlider : public CVerticalSlider {
    public:
	CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& _buttons) override;

	StepVerticalSlider(const CRect& rect, IControlListener* listener, int32_t tag,
				    int32_t iMinPos, int32_t iMaxPos, CBitmap* handle,
				    CBitmap* background, int step);

    private:
	double step;
	double prevValue;
};

}  // namespace HelloWorld
}  // namespace Steinberg
