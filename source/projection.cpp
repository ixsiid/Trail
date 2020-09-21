#include "../include/projection.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

Projection::Projection() {
	points = new CPoint[32];
	points[0] = CPoint(0.0, 512.0);
	points[1] = CPoint(512.0, 0.0);
	count = 2;

	latest = {0, 0};
}

Projection::~Projection() {
	delete points;
}

void Projection::setPointList(CDrawContext::PointList* newPoints) {
	count = newPoints->size();
	for(size_t i=0; i<count; i++) points[i] = (*newPoints)[i];
}

double Projection::evaluate(double input) {
	size_t p;
	input *= 2;
	for (p = 1; p < count; p++)
		if (input < points[p].x) break;

	CPoint dp	    = points[p] - points[p - 1];
	double rate   = (input - points[p - 1].x) / dp.x;
	double output = rate * dp.y + points[p - 1].y;

	latest = {input, output};
	return output;
}

}  // namespace HelloWorld
}  // namespace Steinberg