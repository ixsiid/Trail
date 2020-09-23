#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace HelloWorld {

class DFT {
    public:
	DFT(int num);
	~DFT();

	void process(float* source);
	float* spectrum;
	float* fpeak;

	double f0;
	float noise_level;

    private:
	int num;

	float* re;
	float* im;

	int* index;
	float* w_re;
	float* w_im;
};

}  // namespace HelloWorld
}  // namespace Steinberg