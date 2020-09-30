#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace HelloWorld {

struct Point {
	size_t index;
	float value;
};

class DFT {
    public:
	DFT(size_t num, double sampleRate);
	~DFT();

	void process(float* source);
	tresult setSampleNum(size_t num);
	tresult setSampleRate(double sampleRate);
	tresult calculateF0Range(size_t num, double sampleRate);

	float* spectrum;
	Point* fpeak;
	size_t fpeakCount;

	double f0;
	float noise_level;
	size_t rangeF0Search;

    private:
	const size_t MaxF0Range = 512;
	size_t num;

	float* re;
	float* im;

	int* index;
	float* w_re;
	float* w_im;

	double sampleRate;
};

}  // namespace HelloWorld
}  // namespace Steinberg