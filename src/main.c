#include <stdlib.h>
#include <stdio.h>

#include "window/window.h"
#include "vulkan/vulkan_device.h"
#include "util/defines.h"
#include "engine/engine.h"
#include "editor/editor.h"

struct Window* window;
struct Engine* engine;
struct VulkanDevice* vulkanDevice;
#ifdef EDITOR
struct Editor* editor;
#endif

#ifndef DISABLE_THREADS
HANDLE updateStartSemaphore, updateFinishedSemaphore;

DWORD WINAPI updateThreadFunc(void* data) {
	DWORD dWaitResult;
	while (1) {
		dWaitResult = WaitForSingleObject(updateStartSemaphore, 0L);
		if (dWaitResult == WAIT_OBJECT_0) {
			engineUpdate(engine, vulkanDevice, window);

			ReleaseSemaphore(updateFinishedSemaphore, 1, 0);
		}
	}
	return 0;
}
#endif

void syncedUpdate() {
	engineSyncedUpdate(engine, vulkanDevice, window);
}

void render() {
	engineRender(engine, vulkanDevice);
#ifdef EDITOR
	editorFrame(editor);
#endif
	enginePresent(engine, vulkanDevice);
}

int main() {
	recompileSpirv();

	window = windowInit(window);
	vulkanDevice = vulkanDeviceInit(window);
	window->surfaceWidth = (int)vulkanDevice->swapchain.extent.width;
	window->surfaceHeight = (int)vulkanDevice->swapchain.extent.height;
	engine = engineInit(vulkanDevice);

#ifndef DISABLE_THREADS
	HANDLE updateThread = CreateThread(NULL, 0, updateThreadFunc, NULL, 0, NULL);
	updateStartSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
	updateFinishedSemaphore = CreateSemaphore(NULL, 0, 1, NULL);

	DWORD dWaitResult;
#endif

	engine->writeIndex = 0;
	engine->readIndex = 0;
	engine->bufferWriteIndex = 0;
	engine->bufferReadIndex = 0;
	vulkanDevice->writeIndex = engine->writeIndex;
	vulkanDevice->readIndex = engine->readIndex;
	vulkanDevice->bufferWriteIndex = engine->bufferWriteIndex;
	vulkanDevice->bufferReadIndex = engine->bufferReadIndex;

#ifdef EDITOR
	editor = editorInit(engine, vulkanDevice, window);
#endif

#if THREAD_COUNT == 2
	engineSyncedUpdate(engine, vulkanDevice, window);
	engineUpdate(engine, vulkanDevice, window);

	++engine->writeIndex;
	++engine->bufferWriteIndex;
#endif

	// test();

	while (windowLoop(window)) {
		vulkanDevice->writeIndex = engine->writeIndex;
		vulkanDevice->readIndex = engine->readIndex;
		vulkanDevice->bufferWriteIndex = engine->bufferWriteIndex;
		vulkanDevice->bufferReadIndex = engine->bufferReadIndex;

		syncedUpdate(engine);

#ifndef DISABLE_THREADS
		// Start async update
		ReleaseSemaphore(updateStartSemaphore, 1, 0);
#else
		engineUpdate(engine, vulkanDevice, window);
#endif

		render(engine, vulkanDevice);

		engine->writeIndex = (engine->writeIndex + 1) % THREAD_COUNT;
		engine->readIndex = (engine->readIndex + 1) % THREAD_COUNT;
		engine->bufferWriteIndex = (engine->bufferWriteIndex + 1) % IMAGE_COUNT;
		engine->bufferReadIndex = (engine->bufferReadIndex + 1) % IMAGE_COUNT;

#ifndef DISABLE_THREADS
		// Wait for update to finish
		do {
			dWaitResult = WaitForSingleObject(updateFinishedSemaphore, 1L);
		} while (dWaitResult != WAIT_OBJECT_0);
#endif
	}

#ifndef DISABLE_THREADS
	CloseHandle(updateFinishedSemaphore);
	CloseHandle(updateStartSemaphore);
	CloseHandle(updateThread);
#endif

	vkDeviceWaitIdle(vulkanDevice->device);
#ifdef EDITOR
	editorDestroy(editor);
#endif
	vulkanDeviceDestroy(vulkanDevice);
	engineDestroy(engine);
	windowDestroy(window);
	return 0;
}
