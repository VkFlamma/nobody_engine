#ifndef EDITOR_H
#define EDITOR_H

#include <stdbool.h>
#include <stdint.h>

#include "containers/vector.h"

#define EDITOR_WINDOW_TITLE_HEIGHT 10
#define EDITOR_WINDOW_BORDER_WIDTH 7
#define EDITOR_FONT_SIZE_X 10
#define EDITOR_FONT_SIZE_Y 14
#define EDITOR_FONT_SIZE_ADVANCE (EDITOR_FONT_SIZE_X + 1)
#define WINDOW_RESIZE_SIZE 13
#define WINDOW_CLOSE_SIZE 18

struct Engine;
struct VulkanDevice;
struct Window;
struct Texture;

struct EditorPoint {
	int x;
	int y;
};

void editorPointReset(struct EditorPoint* point);
void editorPointAddTo(struct EditorPoint* dest, struct EditorPoint add);

struct Editor {
	struct Engine* engine;
	struct VulkanDevice* vk;
	struct Window* window;

	struct Texture* fontTexture;

	struct Vector windows;

	struct EditorPoint position;

	bool draw;
};

struct Editor* editor;

struct Editor* editorInit(struct Engine* engine, struct VulkanDevice* vk, struct Window* window);
void editorDestroy();

void editorFrame();

bool editorDrawWindow(const char* windowName);
void editorDrawQuad(struct EditorPoint pos, struct EditorPoint size, uint32_t index, uint32_t specialization);
void editorDrawText(struct EditorPoint pos, struct EditorPoint max, const char* text);

#endif // EDITOR_H
