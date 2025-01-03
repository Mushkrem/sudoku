#pragma once

#include "interface.h"
#include "utils.h"

int read_resources_impl(_label** label) {
	return 1;
}

void draw_text_impl(COORD position, const wchar_t* text, ...) {
	wchar_t buffer[256];
	va_list args;
	va_start(args, text);
	Utils.write(&buffer, sizeof(buffer), text, args);
	va_end(args);

	Utils.append(&buffer, sizeof(buffer), L"\033[0m"); // reset any formatting

	Utils.print(buffer, position);
}

UINTERFACE Interface = {
	.draw_text = draw_text_impl
};