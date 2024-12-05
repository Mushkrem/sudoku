#include "utils.h"
#include <stdio.h>

void OutputFormattedDebugString(const wchar_t* format, ...) {
	wchar_t buffer[256];
	va_list args;
	va_start(args, format);
	vswprintf(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
	va_end(args);

	wcscat_s(buffer, sizeof(buffer) / sizeof(wchar_t), L"\n");
	OutputDebugString(buffer);
}

Utility Utils = {
	.Debug = OutputFormattedDebugString
};