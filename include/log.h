#pragma once

#include <public.sdk/source/vst/vstaudioeffect.h>

#define _ACTIVATE_LOG

#ifdef _ACTIVATE_LOG
#define LOG(x) Log::io->write(x)
#else
#define LOG(x)
#endif

namespace Steinberg {
namespace HelloWorld {

class Log {
    public:
	Log();
#ifdef _ACTIVATE_LOG
	~Log();
	static Log* io;

	void write(char* text);

    private:
	FILE* fp;
#endif
};

}  // namespace HelloWorld
}  // namespace Steinberg