#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include "util/env_defines.h"
#include "input.h"

struct Window {
	HINSTANCE hInstance;
	HWND hwnd;
	struct Keyboard keyboard;
	struct Mouse mouse;
	int x;
	int y;
	int width;
	int height;
	int surfaceWidth;
	int surfaceHeight;
	bool close;
};

struct Window* windowInit();
void windowDestroy(struct Window* window);

bool windowLoop(struct Window* window);

#endif // WINDOW_H
