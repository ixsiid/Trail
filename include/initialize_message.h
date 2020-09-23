#pragma once

namespace Steinberg {
namespace HelloWorld {

class DFT;
class Projection;

struct InitializeMessage {
    public:
	DFT* dft;
	Projection* projection;

	float midi_cc_num;
	float noise_level;
};

}  // namespace HelloWorld
}  // namespace Steinberg