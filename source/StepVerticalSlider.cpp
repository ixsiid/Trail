#include "../include/StepVerticalSlider.h"

#include "../include/log.h"

using namespace VSTGUI;

Steinberg::HelloWorld::StepVerticalSlider::StepVerticalSlider(const CRect& rect, IControlListener* listener, int32_t tag,
												  int32_t iMinPos, int32_t iMaxPos, CBitmap* handle,
												  CBitmap* background, int step)
    : CVerticalSlider(rect, listener, tag, iMinPos, iMaxPos, handle, background, CPoint(0, 0), 32) {
	this->step = step;
	this->prevValue = -1.0;
}

CMouseEventResult Steinberg::HelloWorld::StepVerticalSlider::onMouseMoved(CPoint& where, const CButtonState& _buttons) {
	if (_buttons.isLeftButton() && isEditing()) {
		CRect size    = this->getViewSize();
		CPoint handle = this->getHandleSizePrivate();
		double rate   = 1.0 - (where.y - size.top) / (size.bottom - size.top - handle.y);
		double value = floor(rate * step + 0.5) / step;
		value = value > 1.0 ? 1.0 : value < 0.0 ? 0.0 : value;

		LOGN("top: %f, bottom: %f, handle: %f, rate: %f, value: %f", size.top, size.bottom, handle.y, rate, value);

		if (value == prevValue) return kMouseEventHandled;

		setValueNormalized(value);

		if (isDirty()) {
			valueChanged();
			invalid();
		}
		return kMouseEventHandled;
	}
	return kMouseEventNotHandled;
}