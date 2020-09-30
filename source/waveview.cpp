#include "../include/waveview.h"

#include "../include/dft.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

WaveView::WaveView(const CRect& rect, DFT* dft) : CControl(rect) {
	this->k = -8.0;
	this->p = 160.0;

	this->dft = dft;

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

	// スペクトル描画
	double ax = 512 / dft->rangeF0Search;
	context->setFrameColor(CColor(19, 193, 54, 255));
	for (size_t x = 0; x < 256; ++x)
		points[x] = CPoint(x * ax, dft->spectrum[x] * k + p);
	context->drawPolygon(points);

	// 包括線描画
	context->setFrameColor(CColor(193, 19, 54, 255));
	CDrawContext::PointList peakPoints(dft->fpeakCount);
	peakPoints[0] = CPoint(dft->fpeak[0].index * ax, dft->fpeak[0].value * k + p);
	for (size_t x = 1; x < dft->fpeakCount; ++x)
		peakPoints[x] = CPoint(dft->fpeak[dft->fpeakCount - x].index * ax,
		                       dft->fpeak[dft->fpeakCount - x].value * k + p);
	context->drawPolygon(peakPoints);

	context->setFrameColor(CColor(19, 19, 254, 255));
	CPoint s = {dft->f0 * 2.0, 0.0};
	CPoint f = {dft->f0 * 2.0, 10 + 256 + 80256};
	context->drawLine(s, f);

	setDirty(false);
}

void WaveView::update() { invalid(); }

}  // namespace HelloWorld
}  // namespace Steinberg