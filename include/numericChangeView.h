#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "../include/plugids.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class NumericChangeView : public CControl {
    public:
	NumericChangeView(const CRect& rect, IControlListener * listener, int32_t tag, char* bitmap, CRect bmpRect, int value, int min, int max);
	~NumericChangeView();

	void draw(CDrawContext* context) override;

	void update();

	CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) override;

	int getValue();

	CLASS_METHODS(NumericChangeView, CControl);

    private:
	int current, min, max;
	int leftClick, rightClick;

	CCoord right, left;
	CGraphicsTransform textTransform;

	CBitmap *allow;
	CRect leftAllow, rightAllow;
	CPoint left_on, left_off, right_on, right_off;

	char text[4];
};

}  // namespace HelloWorld
}  // namespace Steinberg