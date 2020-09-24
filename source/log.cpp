#include "../include/log.h"

namespace Steinberg {
namespace HelloWorld {


Log::Log() {
#ifdef _ACTIVATE_LOG
	fp = fopen("C:\\Users\\tal\\vst_sample.txt", "a");
	if (Log::io) return;

	Log::io = this;
	write("//----  Logging Start ----//\n");
#endif
}

#ifdef _ACTIVATE_LOG
Log* Log::io = nullptr;
char Log::buffer[512];

Log::~Log() {
	write("// **** Logging Finish ----//\n");
	fclose(fp);
}

void Log::write(char* text) {
	fwrite(text, 1, strlen(text), fp);
	fflush(fp);
}
#endif

}  // namespace HelloWorld
}  // namespace Steinberg
