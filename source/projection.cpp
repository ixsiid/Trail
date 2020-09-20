#include "../include/projection.h"

#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

Projection::Projection(const CRect& rect) : CControl(rect) {
	top	= rect.top;
	left = rect.left;

	_points	 = new CPoint[32];
	_points[0] = CPoint(0, 512);
	_points[1] = CPoint(512, 0);
	_points[2] = CPoint(128, 128);
	count	 = 3;

	sortPoints();

	m			  = CPoint(256, 256);
	showMousePointer = false;
	drag			  = false;
}

CMouseEventResult Projection::onMouseDown(CPoint& pos, const CButtonState& buttons) {
	if (buttons.isDoubleClick() && buttons.isLeftButton()) {
		if (count < 32) {
			_points[count] = pos;
			_points[count].x -= left;
			_points[count].y -= top;
			count++;

			sortPoints();
		}
		return CMouseEventResult::kMouseEventHandled;
	} else if (active >= 0 && buttons.isLeftButton()) {
		drag = true;
		return CMouseEventResult::kMouseEventHandled;
	} else if (active >= 2 && buttons.isRightButton()) {
		for (int i = active; i < count - 1; i++) {
			_points[i] = _points[i + 1];
		}
		active = -1;
		count--;

		sortPoints();
		return CMouseEventResult::kMouseEventHandled;
	}

	return CMouseEventResult::kMouseEventNotHandled;
}

CMouseEventResult Projection::onMouseUp(CPoint& where, const CButtonState& buttons) {
	drag = false;
	return CMouseEventResult::kMouseEventHandled;
}

CMouseEventResult Projection::onMouseMoved(CPoint& pos, const CButtonState& buttons) {
	m = pos;
	m.x -= left;
	m.y -= top;

	if (drag) {
		_points[active] = m;
		if (active == 0) _points[0].x = 0;
		if (active == 1) _points[1].x = 512;

		sortPoints();
		return CMouseEventResult::kMouseEventHandled;
	}

	active = -1;
	for (int i = 0; i < count; i++) {
		CCoord dx = m.x - _points[i].x;
		CCoord dy = m.y - _points[i].y;
		CCoord d	= dx * dx + dy * dy;
		if (d < 49) {
			active = i;
			break;
		}
	}

	return CMouseEventResult::kMouseEventHandled;
}

CMouseEventResult Projection::onMouseEntered(CPoint& where, const CButtonState& buttons) {
	showMousePointer = true;
	return CMouseEventResult::kMouseEventNotHandled;
}
CMouseEventResult Projection::onMouseExited(CPoint& where, const CButtonState& buttons) {
	showMousePointer = false;
	drag			  = false;
	return CMouseEventResult::kMouseEventNotHandled;
}

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

	context->drawPolygon(points);

	for (int i = 0; i < count; i++) {
		context->drawEllipse(CRect(_points[i].x - 3, _points[i].y - 3, _points[i].x + 3, _points[i].y + 3), CDrawStyle::kDrawStroked);
	}

	if (active >= 0) {
		context->setFillColor(CColor(180, 180, 180, 255));
		CPoint* p = &_points[active];
		context->drawEllipse(CRect(p->x - 3, p->y - 3, p->x + 3, p->y + 3), CDrawStyle::kDrawFilled);
		context->drawLine(CPoint(0, p->y), CPoint(512, p->y));
		context->drawLine(CPoint(p->x, 0), CPoint(p->x, 512));
	} else if (showMousePointer) {
		context->setFillColor(CColor(180, 180, 180, 255));
		context->drawEllipse(CRect(m.x - 3, m.y - 3, m.x + 3, m.y + 3), CDrawStyle::kDrawFilled);
	}

	context->setFrameColor(CColor(19, 19, 254, 255));
	CPoint s = {DFT::f0 * 2.0, 0.0};
	CPoint f = {DFT::f0 * 2.0, 512.0};
	context->drawLine(s, f);

	context->setFrameColor(CColor(193, 193, 254, 255));
	context->drawRect(CRect(halfBorderWidth, halfBorderWidth, width, height), kDrawStroked);
	setDirty(false);
}

void Projection::sortPoints() {
	points.resize(count);

	int index[32];
	for (int i = 0; i < count - 2; i++) index[i] = i + 2;

	for (int i = 0; i < count - 3; i++) {
		for (int k = i; k < count - 3; k++) {
			if (_points[index[i]].x > _points[index[k]].x) {
				int t    = index[k];
				index[k] = index[i];
				index[i] = t;
			}
		}
	}

	points[0]		   = _points[0];
	points[count - 1] = _points[1];
	for (int i = 1; i < count - 1; i++) {
		points[i] = _points[index[i - 1]];
	}
}

void Projection::update() { invalid(); }

}  // namespace HelloWorld
}  // namespace Steinberg