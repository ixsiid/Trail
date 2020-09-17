#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class WaveView : public CControl {
    public:
	WaveView(const CRect& rect, int num);

	void draw(CDrawContext* context) override;

	CLASS_METHODS(WaveView, CControl);

	double* buffer;

	void update();

    protected:
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;

    private:
	int num;
	int offset;
	CDrawContext * context;
};

}  // namespace HelloWorld
}  // namespace Steinberg