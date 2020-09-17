#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg {
namespace HelloWorld {

class DFT {
    public:
	DFT();
	~DFT();

	void process(float* source);
	float* spectrum;

    private:
	float* _p;
	float* re;
	float* im;
};

}  // namespace HelloWorld
}  // namespace Steinberg