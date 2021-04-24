#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

#define KEY_COUNT 44
#define BUTTON_COUNT 2

enum Key {
	KEY_SPACE,
	KEY_ZERO,
	KEY_ONE,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_FIVE,
	KEY_SIX,
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
};

enum KeyState {
	KEY_STATE_PRESS,
	KEY_STATE_REPEAT,
	KEY_STATE_RELEASE,
	KEY_STATE_UP,
	KEY_STATE_AFTER_PRESS,
};

enum Button {
	BUTTON_LEFT,
	BUTTON_RIGHT,
};

enum ButtonState {
	BUTTON_STATE_PRESS,
	BUTTON_STATE_DOWN,
	BUTTON_STATE_RELEASE,
	BUTTON_STATE_UP,
};


struct KeyboardKey {
	enum KeyState keyState;
};

void keyboardKeyUpdate(struct KeyboardKey* keyboardKey, bool d, bool r);
void keyboardKeyReset(struct KeyboardKey* keyboardKey);

struct Keyboard {
	struct KeyboardKey keyboardKeys[KEY_COUNT];
};

void keyboardInit(struct Keyboard* keyboard);
void keyboardUpdate(struct Keyboard* keyboard, enum Key key, bool down, bool repeat);
void keyboardReset(struct Keyboard* keyboard);
bool keyboardWasPressed(struct Keyboard* keyboard, enum Key key);
bool keyboardIsDown(struct Keyboard* keyboard, enum Key key);

struct MouseButton {
	enum ButtonState buttonState;
};

void mouseButtonInit(struct MouseButton* mouseButton);
void mouseButtonUpdate(struct MouseButton* mouseButton, bool d);
void mouseButtonReset(struct MouseButton* mouseButton);

struct Mouse {
	int x;
	int y;
	int dx;
	int dy;
	float dz;
	struct MouseButton mouseButtons[BUTTON_COUNT];
};

void mouseInit(struct Mouse* mouse, int x, int y);
void mouseUpdate(struct Mouse* mouse, enum Button button, bool down);
void mouseWheelUpdate(struct Mouse* mouse, float scroll);
void mouseReset(struct Mouse* mouse);
void mouseMove(struct Mouse* mouse, int x, int y);
bool mouseWasPressed(struct Mouse* mouse, enum Button key);
bool mouseIsDown(struct Mouse* mouse, enum Button key);

#endif // INPUT_H
