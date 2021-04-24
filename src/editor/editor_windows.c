#include "editor/editor_windows.h"

#include "editor/editor_widgets.h"

void editorWindows() {
	if (editorDrawWindow("Test")) {
		editorText("Hello world");
		editorText("whats going on");
		struct EditorPoint size = { 100, 30 };
		editorButton(size, "Test");
	}
}
