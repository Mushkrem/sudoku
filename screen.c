#pragma once

#include "screen.h"

int go_to(COORD c) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, (COORD){ c.X, c.Y });
}

int update() {
	system("cls");

	return EXIT_SUCCESS;
}