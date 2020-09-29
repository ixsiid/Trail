#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class StepVerticalSlider : public CVerticalSlider {
	CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& _buttons) override;
};

}  // namespace HelloWorld
}  // namespace Steinberg
