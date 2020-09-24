#pragma once

#include <public.sdk/source/vst/vstaudioeffect.h>

// #define _ACTIVATE_LOG

#ifdef _ACTIVATE_LOG
#define LOG(x) Log::io->write(x "\n")
#define LOGN(format, ...) sprintf(Log::buffer, format "\n", ##__VA_ARGS__);\
                  Log::io->write(Log::buffer);
#else
#define LOG(x)
#define LOGN(...)
#endif

namespace Steinberg {
namespace HelloWorld {

class Log {
    public:
	Log();
#ifdef _ACTIVATE_LOG
	~Log();
	static Log* io;
	static char buffer[512];

	void write(char* text);

    private:
	FILE* fp;
#endif
};

}  // namespace HelloWorld
}  // namespace Steinberg