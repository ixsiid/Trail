#include "../include/StepVerticalSlider.h"

using namespace VSTGUI;
CMouseEventResult Steinberg::HelloWorld::StepVerticalSlider::onMouseMoved(CPoint& where, const CButtonState& _buttons) {
	if (_buttons.isLeftButton() && isEditing()) {
		this->listener->
		this->getTag();
		float normValue;
		if (isStyleHorizontal())
			normValue = (float)(where.x - impl->meDelta) / (float)impl->rangeHandle;
		else
			normValue = (float)(where.y - impl->meDelta) / (float)impl->rangeHandle;

		if (isStyleRight() || isStyleBottom())
			normValue = 1.f - normValue;

		if (buttons & kZoomModifier)
			normValue = impl->mePreviousVal + ((normValue - impl->mePreviousVal) / impl->zoomFactor);

		setValueNormalized(normValue);

		if (isDirty()) {
			valueChanged();
			invalid();
		}
		return kMouseEventHandled;
	}
	return kMouseEventNotHandled;
}