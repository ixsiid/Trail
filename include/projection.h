#pragma once

#include <public.sdk/source/vst/vstaudioeffect.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class Projection {
    public:
	Projection();
	~Projection();
	void setPointList(CDrawContext::PointList* points);
	double evaluate(double input);

	CPoint latest;

    private:
	CPoint* points;
	size_t count;
};

}  // namespace HelloWorld
}  // namespace Steinberg