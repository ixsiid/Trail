#include "../include/projection.h"

#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {

using namespace VSTGUI;

Projection::Projection() {
	points	= new CPoint[32];
	points[0] = CPoint(0.0, 512.0);
	points[1] = CPoint(512.0, 0.0);
	count	= 2;

	latest = {0, 0};
}

Projection::~Projection() {
	delete points;
}

void Projection::setPointList(CDrawContext::PointList* newPoints) {
	count = newPoints->size();
	for (size_t i = 0; i < count; i++) points[i] = (*newPoints)[i];
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

bool Projection::save(IBStreamer* streamer) {
	streamer->writeInt32u(count);

	double* xy = new double[64];
	for (int i = 0; i < 32; i++) {
		xy[i * 2 + 0] = points[i].x;
		xy[i * 2 + 1] = points[i].y;
	}
	streamer->writeDoubleArray(xy, 64);
	delete[] xy;

	return true;
}

bool Projection::load(IBStreamer* streamer) {
	uint32 d;
	if (!streamer->readInt32u(d)) return false;
	count = d;

	double buffer[64];
	if (!streamer->readDoubleArray(buffer, 64)) return false;
	for (int i = 0; i < count; i++) {
		points[i].x = buffer[i * 2 + 0];
		points[i].y = buffer[i * 2 + 1];
	}

	return true;
}

size_t Projection::getPointList(CPoint* points) {
	for (int i = 1; i < count-1; i++) {
		points[i+1] = this->points[i];
	}
	points[0] = this->points[0];
	points[1] = this->points[count - 1];

	return count;
}

}  // namespace HelloWorld
}  // namespace Steinberg