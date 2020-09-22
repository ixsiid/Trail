#pragma once

#include <base/source/fstreamer.h>
#include <public.sdk/source/vst/vstaudioeffect.h>
#include <public.sdk/source/vst/vstguieditor.h>

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

class Projection {
    public:
	Projection();
	~Projection();
	size_t getPointList(CPoint* points);
	void setPointList(CDrawContext::PointList* points);
	double evaluate(double input);

	CPoint latest;

	bool save(IBStreamer* streamer);
	bool load(IBStreamer* streamer);

    private:
	CPoint* points;
	size_t count;
};

}  // namespace HelloWorld
}  // namespace Steinberg