#pragma once

#include "input.h"

void keyboardKeyUpdate(struct KeyboardKey* keyboardKey, bool d, bool r) {
	if (d) {
		if (r) {
			keyboardKey->keyState = KEY_STATE_REPEAT;
		} else {
			keyboardKey->keyState = KEY_STATE_PRESS;
		}
	} else {
		if (keyboardKey->keyState == KEY_STATE_PRESS || keyboardKey->keyState == KEY_STATE_AFTER_PRESS || keyboardKey->keyState == KEY_STATE_REPEAT) {
			keyboardKey->keyState = KEY_STATE_RELEASE;
		} else {
			keyboardKey->keyState = KEY_STATE_UP;
		}
	}
}

void keyboardKeyReset(struct KeyboardKey* keyboardKey) {
	if (keyboardKey->keyState == KEY_STATE_PRESS) {
		keyboardKey->keyState = KEY_STATE_AFTER_PRESS;
	}
	if (keyboardKey->keyState == KEY_STATE_RELEASE) {
		keyboardKey->keyState = KEY_STATE_UP;
	}
}

void keyboardInit(struct Keyboard* keyboard) {
	for (int i = 0; i < KEY_COUNT; ++i) {
		keyboard->keyboardKeys[i].keyState = KEY_STATE_UP;
	}
}

void keyboardUpdate(struct Keyboard* keyboard, enum Key key, bool down, bool repeat) {
	keyboardKeyUpdate(&keyboard->keyboardKeys[key], down, repeat);
}

void keyboardReset(struct Keyboard* keyboard) {
	for (int i = 0; i < KEY_COUNT; ++i) {
		struct KeyboardKey* key = &keyboard->keyboardKeys[i];
		keyboardKeyReset(key);
	}
}

bool keyboardWasPressed(struct Keyboard* keyboard, enum Key key) {
	enum KeyState state = keyboard->keyboardKeys[key].keyState;
	return state == KEY_STATE_PRESS;
}

bool keyboardIsDown(struct Keyboard* keyboard, enum Key key) {
	enum KeyState state = keyboard->keyboardKeys[key].keyState;
	return state == KEY_STATE_REPEAT || state == KEY_STATE_AFTER_PRESS || state == KEY_STATE_PRESS;
}

void mouseButtonInit(struct MouseButton* mouseButton) {
	mouseButton->buttonState = BUTTON_STATE_UP;
}

void mouseButtonUpdate(struct MouseButton* mouseButton, bool d) {
	if (d) {
		mouseButton->buttonState = BUTTON_STATE_PRESS;
	}
	else {
		if (mouseButton->buttonState == BUTTON_STATE_PRESS || mouseButton->buttonState == BUTTON_STATE_DOWN) {
			mouseButton->buttonState = BUTTON_STATE_RELEASE;
		}
		else {
			mouseButton->buttonState = BUTTON_STATE_UP;
		}
	}
}

void mouseButtonReset(struct MouseButton* mouseButton) {
	if (mouseButton->buttonState == BUTTON_STATE_PRESS) {
		mouseButton->buttonState = BUTTON_STATE_DOWN;
	}
	if (mouseButton->buttonState == BUTTON_STATE_RELEASE) {
		mouseButton->buttonState = BUTTON_STATE_UP;
	}
}

void mouseInit(struct Mouse* mouse, int x, int y) {
	mouse->x = x;
	mouse->y = y;
	mouse->dx = 0;
	mouse->dy = 0;
	mouse->dz = 0.0f;
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		mouseButtonInit(&mouse->mouseButtons[i]);
	}
}

void mouseUpdate(struct Mouse* mouse, enum Button button, bool down) {
	mouseButtonUpdate(&mouse->mouseButtons[button], down);
}

void mouseWheelUpdate(struct Mouse* mouse, float scroll) {
	mouse->dz += scroll;
}

void mouseReset(struct Mouse* mouse) {
	mouse->dx = 0;
	mouse->dy = 0;
	mouse->dz = 0;
	for (int i = 0; i < BUTTON_COUNT; ++i) {
		struct MouseButton* button = &mouse->mouseButtons[i];
		mouseButtonReset(button);
	}
}

void mouseMove(struct Mouse* mouse, int x, int y) {
	mouse->dx = x - mouse->x;
	mouse->dy = y - mouse->y;
	mouse->x = x;
	mouse->y = y;
}

bool mouseWasPressed(struct Mouse* mouse, enum Button key) {
	enum ButtonState state = mouse->mouseButtons[key].buttonState;
	return state == BUTTON_STATE_PRESS;
}

bool mouseIsDown(struct Mouse* mouse, enum Button key) {
	enum ButtonState state = mouse->mouseButtons[key].buttonState;
	return state == BUTTON_STATE_DOWN || state == BUTTON_STATE_PRESS;
}
