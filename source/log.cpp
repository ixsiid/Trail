#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {


Log::Log() {
#ifndef _ACTIVATE_LOG
	return;
#endif

	fp = fopen("C:\\Users\\tal\\vst_sample.txt", "a");
	if (Log::io) return;

	Log::io = this;
	write("//-------------------//\n");
}

#ifdef _ACTIVATE_LOG
Log* Log::io = nullptr;
char Log::buffer[512];

Log::~Log() {
	fclose(fp);
}

void Log::write(char* text) {
	fwrite(text, 1, strlen(text), fp);
	fflush(fp);
}
#endif

}  // namespace HelloWorld
}  // namespace Steinberg
