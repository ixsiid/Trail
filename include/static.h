#pragma once

namespace Steinberg {
namespace HelloWorld {

class WaveView;
class DFT;

class Static {
    public:
	static int num;
	static WaveView* wave;
	static DFT* dft;
};

}  // namespace HelloWorld
}  // namespace Steinberg