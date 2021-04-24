#ifndef EDITOR_WINDOWS_H
#define EDITOR_WINDOWS_H

#include "editor/editor.h"

struct EditorWindow {
	const char* name;
	struct EditorPoint pos;
	struct EditorPoint size;
	bool opened;
	bool moving;
	bool hovered;
	bool resizing;
};

void editorWindows();

#endif // EDITOR_WINDOWS_H
