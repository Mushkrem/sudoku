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
	Utils.Write(&buffer, sizeof(buffer), text, args);
	Utils.Debug(L"!!>> %s", buffer);
	va_end(args);

	Utils.Append(&buffer, sizeof(buffer), L"\033[0m"); // reset any formatting

	Utils.Print(buffer, position);
}

UINTERFACE Interface = {
	.draw_text = draw_text_impl
};