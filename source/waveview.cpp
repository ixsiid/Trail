#include "../include/waveview.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

WaveView::WaveView(const CRect& rect) : CControl(rect) {
	this->t = Static::num / 512;

	this->k = -3000.0;
	this->p = 240.0;
}

void WaveView::draw(CDrawContext* context) {
	context->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
	CDrawContext::Transform transform(*context, CGraphicsTransform().translate(getViewSize().getTopLeft()));

	setDirty(false);

	const auto width		    = getWidth();
	const auto height		    = getHeight();
	const double borderWidth	    = 2.0;
	const double halfBorderWidth = borderWidth / 2.0;

	context->setLineWidth(borderWidth);
	context->setFillColor(kBlackCColor);
	context->drawRect(CRect(0.0, 0.0, width, height), kDrawFilled);

	context->setLineWidth(2.0);
	context->setLineStyle(lineStyle);
	context->setFrameColor(CColor(19, 193, 54, 255));

	const size_t size = (size_t)(Static::num / t);
	if (points.size() != size) points.resize(size);
	for (size_t x = 0; x < size; ++x)
		points[x] = CPoint((CCoord)x, buffer[x * t] * k + p);

	context->drawPolygon(points);

	context->drawRect(CRect(halfBorderWidth, halfBorderWidth, width, height), kDrawStroked);
}

void WaveView::update() {
	invalid();
}

void WaveView::setBuffer(float* buffer) {
	this->buffer = buffer;
}

}  // namespace HelloWorld
}  // namespace Steinberg