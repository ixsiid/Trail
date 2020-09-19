#pragma once

#include <pluginterfaces/vst/ivstplugview.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class Projection : public CControl, public IViewMouseListener {
    public:
	Projection(const CRect& rect);

	void draw(CDrawContext* context) override;

	void update();

	CMouseEventResult viewOnMouseDown(CView* view, CPoint pos, CButtonState buttons) override;
	CMouseEventResult viewOnMouseUp(CView* view, CPoint pos, CButtonState buttons) override;
	CMouseEventResult viewOnMouseMoved(CView* view, CPoint pos, CButtonState buttons) override;
	CMouseEventResult viewOnMouseCancel(CView* view) override;
	void viewOnMouseEntered (CView* view) override;
	void viewOnMouseExited (CView* view) override;
	void viewOnMouseEnabled (CView* view, bool state) override;

	CLASS_METHODS(Projection, CControl);

    protected:

    private:
	CLineStyle lineStyle{CLineStyle::kLineCapRound, CLineStyle::kLineJoinRound};
	CDrawContext::PointList points;
	CPoint * _points;
	int count;
	int active;
	void sortPoints();
};

}  // namespace HelloWorld
}  // namespace Steinberg