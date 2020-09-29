#pragma once

namespace Steinberg {
namespace HelloWorld {

enum HelloWorldParams : Vst::ParamID
{
	kParamFp = 2001,
	kParamF0 = 2000,

	kCCId = 103,
	kNoiseLevel = 104,
	kPrecision = 105,
};


// HERE you have to define new unique class ids: for processor and for controller
// you can use GUID creator tools like https://www.guidgenerator.com/
static const FUID MyProcessorUID (0x2AE4996E, 0x6E70422C, 0xB62464D7, 0xDBEBC064);
static const FUID MyControllerUID (0x80CB603C, 0x0E9A4766, 0xA708019E, 0x292CC863);

//------------------------------------------------------------------------
} // namespace HelloWorld
} // namespace Steinberg
