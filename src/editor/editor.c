#include "editor/editor.h"

#include <stdlib.h>
#include <assert.h>

#include "engine/engine.h"
#include "vulkan/vulkan_device.h"
#include "window/window.h"
#include "math/vec3.h"
#include "vulkan/texture.h"
#include "util/paths.h"
#include "editor/editor_windows.h"

void editorPointReset(struct EditorPoint* point) {
	point->x = 0;
	point->y = 0;
}

void editorPointAddTo(struct EditorPoint* dest, struct EditorPoint add) {
	dest->x += add.x;
	dest->y += add.y;
}

int getFontIndex(char c) {
	switch (c) {
	case 'A': return 0;
	case 'B': return 1;
	case 'C': return 2;
	case 'D': return 3;
	case 'E': return 4;
	case 'F': return 5;
	case 'G': return 6;
	case 'H': return 7;
	case 'I': return 8;
	case 'J': return 9;
	case 'K': return 10;
	case 'L': return 11;
	case 'M': return 12;
	case 'N': return 13;
	case 'O': return 14;
	case 'P': return 15;
	case 'Q': return 16;
	case 'R': return 17;
	case 'S': return 18;
	case 'T': return 19;
	case 'U': return 20;
	case 'V': return 21;
	case 'W': return 22;
	case 'X': return 23;
	case 'Y': return 24;
	case 'Z': return 25;
	case 'a': return 26;
	case 'b': return 27;
	case 'c': return 28;
	case 'd': return 29;
	case 'e': return 30;
	case 'f': return 31;
	case 'g': return 32;
	case 'h': return 33;
	case 'i': return 34;
	case 'j': return 35;
	case 'k': return 36;
	case 'l': return 37;
	case 'm': return 38;
	case 'n': return 39;
	case 'o': return 40;
	case 'p': return 41;
	case 'q': return 42;
	case 'r': return 43;
	case 's': return 44;
	case 't': return 45;
	case 'u': return 46;
	case 'v': return 47;
	case 'w': return 48;
	case 'x': return 49;
	case 'y': return 50;
	case 'z': return 51;
	case '0': return 52;
	case '1': return 53;
	case '2': return 54;
	case '3': return 55;
	case '4': return 56;
	case '5': return 57;
	case '6': return 58;
	case '7': return 59;
	case '8': return 60;
	case '9': return 61;
	default: assert(0);
	}
	return 0;
}

void editorDrawQuad(struct EditorPoint pos, struct EditorPoint size, uint32_t index, uint32_t specialization) {
	struct GuiVertex vertices[GUI_VERTEX_COUNT];
	for (int i = 0; i < GUI_VERTEX_COUNT; ++i) {
		float vx = (float)((i >> 1) & 1);
		float vy = (float)(i & 1);
		vx = vx * size.x + pos.x;
		vy = vy * size.y + pos.y;
		vx = vx / (float)editor->window->surfaceWidth * 2.0f - 1.0f;
		vy = vy / (float)editor->window->surfaceHeight * 2.0f - 1.0f;
		vertices[i].x = vx;
		vertices[i].y = vy;
		vertices[i].sizex = (float)size.x;
		vertices[i].sizey = (float)size.y;
		vertices[i].index = index;
	}

	struct RenderContext* renderContext = guiPipelineGetRenderContext(&editor->vk->guiPipeline);
	renderContext->specialization = specialization;
	guiPipelineAddToQueue(&editor->vk->guiPipeline, (void*)&vertices);
}

void editorDrawText(struct EditorPoint pos, struct EditorPoint max, const char* text) {
	for (int i = 0; text[i] != '\0'; ++i) {
		if (text[i] == ' ') {
			continue;
		}
		struct EditorPoint start, size;
		start.x = pos.x + EDITOR_FONT_SIZE_ADVANCE * i;
		start.y = pos.y;
		size.x = EDITOR_FONT_SIZE_X;
		size.y = EDITOR_FONT_SIZE_Y;
		if (start.x >= max.x || start.y >= max.y) {
			return;
		}
		if (start.x + size.x > max.x) {
			size.x = max.x - start.x;
		}
		if (start.y + size.y > max.y) {
			size.y = max.y - start.y;
		}
		editorDrawQuad(start, size, getFontIndex(text[i]), GUI_FONT);
	}
}

struct EditorWindow* editorInitWindow(const char* windowName) {
	struct EditorWindow* current = vectorAddAndGet(&editor->windows);
	current->name = windowName;
	current->pos.x = 100;
	current->pos.y = 100;
	current->size.x = 276 * 3;
	current->size.y = 128 * 3;
	current->opened = true;
	current->moving = false;
	current->hovered = false;
	current->resizing = false;
	return current;
}

bool editorDrawWindow(const char* windowName) {
	struct EditorWindow* current = NULL;
	for (int i = 0; i < editor->windows.count; ++i) {
		struct EditorWindow* w = vectorGet(&editor->windows, i);
		if (w->name == windowName) {
			current = w;
			break;
		}
	}
	if (!current) {
		current = editorInitWindow(windowName);
	}
	if (!current->opened) {
		return false;
	}

	editorDrawQuad(current->pos, current->size, 0, GUI_WINDOW);
	struct EditorPoint pos, max;
	pos.x = current->pos.x + EDITOR_WINDOW_TITLE_HEIGHT / 2;
	pos.y = current->pos.y + EDITOR_WINDOW_TITLE_HEIGHT / 2;
	max.x = current->pos.x + current->size.x;
	max.y = current->pos.y + current->size.y;
	editorDrawText(pos, max, current->name);
	
	editor->position.x = current->pos.x + 10;
	editor->position.y = current->pos.y + 30;

	return true;
}

void editorResizeWindow(struct EditorWindow* w) {
	struct Mouse* mouse = &editor->window->mouse;
	w->size.x += mouse->dx;
	w->size.y += mouse->dy;
	int minSize = EDITOR_WINDOW_TITLE_HEIGHT * 2;
	if (w->size.x < minSize) {
		w->size.x = minSize;
	}
	if (w->size.y < minSize) {
		w->size.y = minSize;
	}
}

bool isInside(struct EditorWindow* w, int x, int y, int xleft, int xright, int ytop, int ybottom) {
	bool a = w->pos.x + xleft < x;
	bool b = x < w->pos.x + w->size.x - xright;
	bool c = w->pos.y + ytop < y;
	bool d = y < w->pos.y + w->size.y - ybottom;
	return w->pos.x + xleft < x && x < w->pos.x + w->size.x - xright && w->pos.y + ytop < y && y < w->pos.y + w->size.y - ybottom;
}

void editorUpdateWindow(struct EditorWindow* w, struct Mouse* mouse) {
	bool insideClose = isInside(w, mouse->x, mouse->y, w->size.x - WINDOW_CLOSE_SIZE, 0, 0, w->size.y - WINDOW_CLOSE_SIZE);
	bool insideResize = isInside(w, mouse->x, mouse->y, w->size.x - WINDOW_RESIZE_SIZE, 0, w->size.y - WINDOW_RESIZE_SIZE, 0);
	bool insideWindow = isInside(w, mouse->x, mouse->y, 0, 0, 0, 0);
	bool mouseDown = mouseIsDown(mouse, BUTTON_LEFT);
	bool mousePressed = mouseWasPressed(mouse, BUTTON_LEFT);
	if (insideClose && mousePressed) {
		w->opened = false;
	} else if ((insideResize || w->resizing && mouseDown) && !w->moving) {
		if (mouseDown) {
			editorResizeWindow(w);
		}
		w->moving = false;
		w->resizing = mouseDown;
	} else if (insideWindow || w->moving && mouseDown) {
		if (mouseDown) {
			if (w->moving) {
				w->pos.x += mouse->dx;
				w->pos.y += mouse->dy;
			}
			w->moving = true;
		}
		else {
			w->moving = false;
		}
	} else {
		w->resizing = false;
		w->moving = false;
	}
	w->hovered = insideWindow;
}

void editorUpdate() {
	struct Mouse* mouse = &editor->window->mouse;
	int x = mouse->x;
	int y = mouse->y;
	for (int i = 0; i < editor->windows.count; ++i) {
		struct EditorWindow* w = vectorGet(&editor->windows, i);
		editorUpdateWindow(w, mouse);
	}

	if (keyboardWasPressed(&editor->window->keyboard, KEY_ONE)) {
		editor->draw = !editor->draw;
	}
}

////////////////////////////////////////////////////////////

struct Editor* editorInit(struct Engine* engine, struct VulkanDevice* vk, struct Window* window) {
	editor = malloc(sizeof(struct Editor));
	assert(editor);

	editor->engine = engine;
	editor->vk = vk;
	editor->window = window;

	editor->draw = true;

	vectorInit(&editor->windows, 256, sizeof(struct EditorWindow));
	editor->fontTexture = malloc(sizeof(struct Texture));
	textureInitColor(editor->fontTexture, vk, RESOURCES("font2.ktx"));

	return editor;
}

void editorDestroy() {
	vectorDestroy(&editor->windows);
	textureDestroy(editor->fontTexture, editor->vk);
	free(editor->fontTexture);
	free(editor);
}

void editorFrame() {
	editorUpdate();

	editor->vk->guiPipeline.queueIndex = 0;
	vulkanDeviceEditorBegin(editor->vk);

	if (editor->draw) {
		editorPointReset(&editor->position);
		editorWindows(editor);
	}

	VkCommandBuffer commandBuffer = editor->vk->commandBuffers.editorCommandBuffers[editor->vk->bufferReadIndex];
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, editor->vk->guiPipeline.layout, 0, 1, &editor->fontTexture->descriptorSet, 0, NULL);
	guiPipelineRender(&editor->vk->guiPipeline);
	vulkanDeviceEditorEnd(editor->vk);
}
