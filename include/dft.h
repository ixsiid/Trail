#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace HelloWorld {

class DFT {
    public:
	static const int num = 8192;

	static void initialize();
	static void destroy();

	static void process(float* source);
	static float* spectrum;
	static float* fpeak;

	static double f0;
	static float lowFreqWeight;

    private:
     static bool initialized;
	static float* re;
	static float* im;

	static int* index;
	static float* w_re;
	static float* w_im;
};

}  // namespace HelloWorld
}  // namespace Steinberg