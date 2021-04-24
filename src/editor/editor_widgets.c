#include "editor/editor_widgets.h"

#include <string.h>

#include "pipelines/gui_pipeline.h"

void editorText(const char* text) {
	int x = editor->position.x;
	int y = editor->position.y;
	struct EditorPoint size;
	size.x = x + EDITOR_FONT_SIZE_ADVANCE * (int)strlen(text);
	size.y = y + EDITOR_FONT_SIZE_Y;
	editorDrawText(editor->position, size, text);
	editor->position.y += EDITOR_FONT_SIZE_Y;
}

bool editorButton(struct EditorPoint size, const char* text) {
	editorDrawQuad(editor->position, size, 0, GUI_BUTTON);
	return true;
}
