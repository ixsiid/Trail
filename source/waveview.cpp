#include "../include/waveview.h"

#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

WaveView::WaveView(const CRect& rect) : CControl(rect) {
	this->k = -16.0;
	this->p = 320.0;

	points.resize(256);
}

void WaveView::draw(CDrawContext* context) {
	context->setDrawMode(CDrawMode(CDrawModeFlags::kAntiAliasing));
	CDrawContext::Transform transform(*context, CGraphicsTransform().translate(getViewSize().getTopLeft()));

	const auto width		    = getWidth();
	const auto height		    = getHeight();
	const double borderWidth	    = 2.0;
	const double halfBorderWidth = borderWidth / 2.0;

	context->setLineWidth(1.0);
	context->setLineStyle(lineStyle);
	context->setFrameColor(CColor(19, 193, 54, 255));

	for (size_t x = 0; x < 256; ++x)
		points[x] = CPoint(DFT::spectrum[x] * k + p, (CCoord)x * 2);

	context->drawPolygon(points);

	context->setFrameColor(CColor(193, 19, 54, 255));
	for (size_t x = 0; x < 256; ++x)
		points[x] = CPoint(DFT::fpeak[x] * k + p, (CCoord)x * 2);

	context->drawPolygon(points);

	context->setFrameColor(CColor(19, 19, 254, 255));
	CPoint s = {0.0, DFT::f0 * 2.0};
	CPoint f = {512.0, DFT::f0 * 2.0};
	context->drawLine(s, f);

	context->setFrameColor(CColor(193, 193, 254, 255));
	context->drawRect(CRect(halfBorderWidth, halfBorderWidth, width, height), kDrawStroked);
	setDirty(false);
}

void WaveView::update() { invalid(); }

}  // namespace HelloWorld
}  // namespace Steinberg