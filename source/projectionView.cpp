#include "../include/projectionView.h"

#include "../include/dft.h"

#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

ProjectionView::ProjectionView(const CRect& rect, Projection * proj) : CControl(rect) {
	this->proj = proj;

	top	= rect.top;
	left = rect.left;

	_points	 = new CPoint[32];
	count = proj->getPointList(_points);

	sortPoints();

	m			  = CPoint(256, 256);
	showMousePointer = false;
	drag			  = false;
}

CMouseEventResult ProjectionView::onMouseDown(CPoint& pos, const CButtonState& buttons) {
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

CMouseEventResult ProjectionView::onMouseUp(CPoint& where, const CButtonState& buttons) {
	drag = false;
	return CMouseEventResult::kMouseEventHandled;
}

CMouseEventResult ProjectionView::onMouseMoved(CPoint& pos, const CButtonState& buttons) {
	m = pos;
	m.x -= left;
	m.y -= top;

	if (drag) {
		if (active == 0) m.x = 0.0;
		if (active == 1) m.x = 512.0;

		if (m.y < 0.0) m.y = 0.0;
		else if (m.y > 512.0) m.y = 512.0;
		if (m.x < 0.0) m.x = 0.0;
		else if (m.x > 512.0) m.x = 512.0;
		
		_points[active] = m;

		sortPoints();
		return CMouseEventResult::kMouseEventHandled;
	}

	active = -1;
	for (int i = 0; i < count; i++) {
		CCoord dx = m.x - _points[i].x;
		CCoord dy = m.y - _points[i].y;
		CCoord d	= dx * dx + dy * dy;
		if (d < 64) {
			active = i;
			break;
		}
	}

	return CMouseEventResult::kMouseEventHandled;
}

CMouseEventResult ProjectionView::onMouseEntered(CPoint& where, const CButtonState& buttons) {
	showMousePointer = true;
	return CMouseEventResult::kMouseEventNotHandled;
}
CMouseEventResult ProjectionView::onMouseExited(CPoint& where, const CButtonState& buttons) {
	showMousePointer = false;
	drag			  = false;
	return CMouseEventResult::kMouseEventNotHandled;
}

void ProjectionView::draw(CDrawContext* context) {
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

	int r = 5;
	if (active >= 0) {
		context->setFillColor(CColor(180, 180, 180, 255));
		CPoint* p = &_points[active];
		context->drawEllipse(CRect(p->x - r, p->y - r, p->x + r, p->y + r), CDrawStyle::kDrawFilled);
		context->drawLine(CPoint(0, p->y), CPoint(512, p->y));
		context->drawLine(CPoint(p->x, 0), CPoint(p->x, 512));
	} else if (showMousePointer) {
		context->setFillColor(CColor(180, 180, 180, 255));
		context->drawEllipse(CRect(m.x - r, m.y - r, m.x + r, m.y + r), CDrawStyle::kDrawFilled);
	}

	context->setFrameColor(CColor(19, 19, 254, 255));
	CPoint s = {proj->latest.x, 0.0};
	CPoint c = {proj->latest.x, proj->latest.y};
	CPoint f = {512.0 + 80, proj->latest.y};
	context->drawLine(s, c);
	context->drawLine(c, f);

	setDirty(false);
}

void ProjectionView::sortPoints() {
	points.resize(count);

	int index[32];
	for (int i = 0; i < count - 2; i++) index[i] = i + 2;

	for (int i = 0; i < count - 3; i++) {
		for (int k = i; k < count - 2; k++) {
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

	proj->setPointList(&points);
}

void ProjectionView::update() { invalid(); }

}  // namespace HelloWorld
}  // namespace Steinberg