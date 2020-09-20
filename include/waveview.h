#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class WaveView : public CControl {
    public:
	WaveView(const CRect& rect);

	void draw(CDrawContext* context) override;

	CLASS_METHODS(WaveView, CControl);

	void update();

	float k;

    protected:
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;

    private:
	float p;
};

}  // namespace HelloWorld
}  // namespace Steinberg