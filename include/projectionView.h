#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

#include "projection.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class ProjectionView : public CControl{
    public:
	ProjectionView(const CRect& rect, Projection * proj);

	void draw(CDrawContext* context) override;

	void update();

	CMouseEventResult onMouseDown (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseUp (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseMoved (CPoint& where, const CButtonState& buttons) override;
//	CMouseEventResult onMouseCancel () override;

	CMouseEventResult onMouseEntered (CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseExited (CPoint& where, const CButtonState& buttons) override;

	

	CLASS_METHODS(ProjectionView, CControl);

    private:
	CCoord top, left;
	CPoint m;
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;
	CPoint* _points;
	int count;
	int active;
	void sortPoints();
	bool showMousePointer;
	bool drag;

	Projection * proj;
};

}  // namespace HelloWorld
}  // namespace Steinberg