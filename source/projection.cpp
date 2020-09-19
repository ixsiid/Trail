#include "../include/projection.h"

#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

Projection::Projection(const CRect& rect) : CControl(rect) {
	_points = new CPoint[32];
	_points[0] = CPoint(0, 512);
	_points[1] = CPoint(512, 0);
	count = 2;

//	this->registerViewMouseListener(this);
}

CMouseEventResult Projection::viewOnMouseDown(CView* view, CPoint pos, CButtonState buttons) {
	if (buttons.isLeftButton()) {
		if (count < 32) _points[count++] = pos;
		return CMouseEventResult::kMouseEventHandled;
	}

	return CMouseEventResult::kMouseEventNotHandled;
}
CMouseEventResult Projection::viewOnMouseUp(CView* view, CPoint pos, CButtonState buttons) {
	return CMouseEventResult::kMouseEventNotHandled;
}
CMouseEventResult Projection::viewOnMouseMoved(CView* view, CPoint pos, CButtonState buttons) {
	return CMouseEventResult::kMouseEventNotHandled;
}
CMouseEventResult Projection::viewOnMouseCancel(CView* view) {
	return CMouseEventResult::kMouseEventNotHandled;
}

void Projection::viewOnMouseEntered(CView* view) {}
void Projection::viewOnMouseExited(CView* view) {}
void Projection::viewOnMouseEnabled(CView* view, bool state) {}

void Projection::draw(CDrawContext* context) {
	context->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
	CDrawContext::Transform transform(*context, CGraphicsTransform().translate(getViewSize().getTopLeft()));

	const auto width		    = getWidth();
	const auto height		    = getHeight();
	const double borderWidth	    = 2.0;
	const double halfBorderWidth = borderWidth / 2.0;

	context->setLineWidth(1.0);
	context->setLineStyle(lineStyle);
	context->setFrameColor(CColor(19, 193, 54, 255));

	sortPoints();
	context->drawPolygon(points);

	context->setFrameColor(CColor(19, 19, 254, 255));
	CPoint s = {0.0, DFT::f0 * 2.0};
	CPoint f = {512.0, DFT::f0 * 2.0};
	context->drawLine(s, f);

	context->setFrameColor(CColor(193, 193, 254, 255));
	context->drawRect(CRect(halfBorderWidth, halfBorderWidth, width, height), kDrawStroked);
	setDirty(false);
}

void Projection::sortPoints() {
	points.resize(count);
	for(int i=0; i<count; i++) {
		points[i] = _points[i];
	}
}

void Projection::update() { invalid(); }

}  // namespace HelloWorld
}  // namespace Steinberg