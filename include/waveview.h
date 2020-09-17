#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "static.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class WaveView : public CControl {
    public:
	WaveView(const CRect& rect);

	void draw(CDrawContext* context) override;

	CLASS_METHODS(WaveView, CControl);

	void update();

	void setBuffer(float * buffer);

    protected:
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;

    private:
	int t;

	float * buffer;
	float k, p;
};

}  // namespace HelloWorld
}  // namespace Steinberg