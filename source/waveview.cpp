#include "../include/waveview.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

WaveView::WaveView(const CRect & rect, int num) : CControl(rect) {
	this->num = num;
	this->buffer = new double[num];

	this->offset = 0;
}

void WaveView::draw(CDrawContext * context) {
	context->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
	CDrawContext::Transform transform(*context, CGraphicsTransform().translate(getViewSize().getTopLeft()));

	setDirty(false);

	const auto width = getWidth();
	const auto height = getHeight();
	const double borderWidth = 2.0;
	const double halfBorderWidth = borderWidth/ 2.0;

	context->setLineWidth(borderWidth);
	context->setFillColor(kBlackCColor);
	context->drawRect(CRect(0.0, 0.0, width, height), kDrawFilled);

	context->setLineWidth(2.0);
	context->setLineStyle(lineStyle);
	context->setFrameColor(CColor(19, 193, 54, 255));
	const size_t size = (size_t)(width + 1.0);
	if (points.size() != size) points.resize(size);
	for(size_t x =0; x<points.size(); ++x)
	points[x] = CPoint((CCoord)x, height * sin(x * 6.28 / width + offset));
	context->drawPolygon(points);
	
	context->drawRect(CRect(halfBorderWidth, halfBorderWidth, width, height), kDrawStroked);
}


void WaveView::update() {
	offset++;
	invalid();
}

}  // namespace HelloWorld
}  // namespace Steinberg