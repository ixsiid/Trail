#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "dft.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class WaveView : public CControl {
    public:
	WaveView(const CRect& rect, DFT * dft);

	void draw(CDrawContext* context) override;

	CLASS_METHODS(WaveView, CControl);

	void update();

    protected:
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;

    private:
	float k, p;
	DFT * dft;
};

}  // namespace HelloWorld
}  // namespace Steinberg