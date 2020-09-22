#include "../include/NumericChangeView.h"

#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

NumericChangeView::NumericChangeView(const CRect& rect, IControlListener * listener, int32_t tag, char* bitmap, CRect bmpRect, int value, int min, int max) : CControl(rect, listener, tag, nullptr) {
	this->current = value;
	this->min	  = min;
	this->max	  = max;

	setMin(min);
	setMax(max);

	left	 = rect.left + bmpRect.right;
	right = rect.right - bmpRect.right;

	leftClick	 = 0;
	rightClick = 0;

	sprintf(text, "%d", current);

	textTransform = CGraphicsTransform().translate(bmpRect.right + 3.0, 1.0);

	allow = new CBitmap(bitmap);
	leftAllow(0, 0, bmpRect.right, bmpRect.bottom);
	rightAllow(rect.right - rect.left - bmpRect.right, 0, rect.right, bmpRect.bottom);
	left_on(0, 0);
	left_off(0, bmpRect.bottom);
	right_on(bmpRect.right, 0);
	right_off(bmpRect.right, bmpRect.bottom);
}

NumericChangeView::~NumericChangeView() {
	allow->forget();
}

CMouseEventResult NumericChangeView::onMouseDown(CPoint& pos, const CButtonState& buttons) {
	if (buttons.isLeftButton()) {
		if (pos.x < left) {
			if (min < current) {
				sprintf(text, "%d", --current);
				leftClick = 3;
				value = current;
				beginEdit();
				valueChanged();
				if (isDirty()) invalid();
				endEdit();
				return CMouseEventResult::kMouseEventHandled;
			}
		} else if (pos.x > right) {
			if (max > current) {
				sprintf(text, "%d", ++current);
				rightClick = 3;
				value = current;
				beginEdit();
				valueChanged();
				if (isDirty()) invalid();
				endEdit();
				return CMouseEventResult::kMouseEventHandled;
			}
		}
	}

	return CMouseEventResult::kMouseEventNotHandled;
}

void NumericChangeView::draw(CDrawContext* context) {
	context->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
	CDrawContext::Transform transform(*context, CGraphicsTransform().translate(getViewSize().getTopLeft()));

	const auto width  = getWidth();
	const auto height = getHeight();

	context->setFillColor(CColor(0, 0, 0, 255));
	context->drawGraphicsPath(context->createTextPath(kNormalFontVeryBig, text),
						 CDrawContext::kPathFilled,
						 &textTransform);

	if (leftClick > 0) {
		leftClick--;
		context->drawBitmap(allow, leftAllow, left_on);
	} else {
		context->drawBitmap(allow, leftAllow, left_off);
	}

	if (rightClick > 0) {
		rightClick--;
		context->drawBitmap(allow, rightAllow, right_on);
	} else {
		context->drawBitmap(allow, rightAllow, right_off);
	}

	setDirty(false);
}

void NumericChangeView::update() { invalid(); }

int NumericChangeView::getValue() {
	return value;
}

}  // namespace HelloWorld
}  // namespace Steinberg