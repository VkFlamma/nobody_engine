#include "engine.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "vulkan/vulkan_device.h"
#include "window/window.h"
#include "util/cube.h"
#include "util/defines.h"
#include "util/paths.h"

void handleKeyboardSynced(struct Engine* engine, struct VulkanDevice* vk, struct Keyboard* keyboard) {
	if (keyboardWasPressed(keyboard, KEY_U)) {
		vk->voxelPipeline.wireframe = !vk->voxelPipeline.wireframe;
		vk->particlesPipeline.wireframe = !vk->particlesPipeline.wireframe;
		reloadPipelines(vk);
	}
	if (keyboardWasPressed(keyboard, KEY_SPACE)) {
		recompileSpirv();
		reloadPipelines(vk);
	}
}

void handleKeyboard(struct Engine* engine, struct VulkanDevice* vk, struct Keyboard* keyboard) {
	if (keyboardWasPressed(keyboard, KEY_SPACE)) {
		storageReload(&engine->storage, vk);
	}

	if (keyboardWasPressed(keyboard, KEY_R)) {
		if (engine->debugWireframeVoxel) {
			engine->debugWireframeVoxel->draw = !engine->debugWireframeVoxel->draw;
		}
	}

	if (engine->activeVoxel) {
		if (keyboardIsDown(keyboard, KEY_A)) {
			engine->activeVoxel->transform.position.x -= MOVEMENT_SPEED;
		}
		if (keyboardIsDown(keyboard, KEY_D)) {
			engine->activeVoxel->transform.position.x += MOVEMENT_SPEED;
		}
		if (keyboardIsDown(keyboard, KEY_W)) {
			engine->activeVoxel->transform.position.z -= MOVEMENT_SPEED;
		}
		if (keyboardIsDown(keyboard, KEY_S)) {
			engine->activeVoxel->transform.position.z += MOVEMENT_SPEED;
		}
		if (keyboardIsDown(keyboard, KEY_Q)) {
			engine->activeVoxel->transform.position.y -= MOVEMENT_SPEED;
		}
		if (keyboardIsDown(keyboard, KEY_E)) {
			engine->activeVoxel->transform.position.y += MOVEMENT_SPEED;
		}
	}
}

void handleMouse(struct Engine* engine, struct VulkanDevice* vk, struct Mouse* mouse) {
	if (mouseIsDown(mouse, BUTTON_LEFT) && (mouse->dx != 0 || mouse->dy != 0)) {
		viewRotate(&vk->view, (float)mouse->dx, (float)mouse->dy);
	}
	if (mouseWasPressed(mouse, BUTTON_LEFT)) {
		struct Animation anim;
		struct Voxel* voxel = (struct Voxel*)storageGetVoxel(&engine->storage, 0);
		vec3_init(&voxel->bones[1].rotationAxis, -1.0f, 0.0f, 0.0f);
		void* target = (void*)&voxel->bones[1].rotationAngle;
		animationInit(&anim, target, 0.0f, 180.0f, 200000);
		animationEngineAdd(&engine->animEngine, &anim);
	}
}

void engineUpdateDebugVoxel(struct Engine* engine) {
	if (!engine->debugWireframeVoxel) return;
	if (engine->debugWireframeVoxel->draw && engine->activeVoxel) {
		engine->debugWireframeVoxel->parent = engine->activeVoxel;
		engine->debugWireframeVoxel->transform.position = engine->activeVoxel->bboxMin;
		vec3_sub(&engine->debugWireframeVoxel->transform.scale, &engine->activeVoxel->bboxMax, &engine->activeVoxel->bboxMin);
	}
}

struct Engine* engineInit(struct VulkanDevice* vk) {
	struct Engine* engine = malloc(sizeof(struct Engine));
	assert(engine);

	cubeInit();
	storageInit(&engine->storage);
	animationEngineInit(&engine->animEngine);

	engine->delta = 0;
	int success = timespec_get(&engine->lastFrameTime, TIME_UTC);

	engine->debugWireframeVoxel = NULL;
	engine->setCameraOnActiveObject = false;

	/*int debugWireframeVoxelIndex = storageAddVoxel(&engine->storage);
	engine->debugWireframeVoxel = storageGetVoxel(&engine->storage, debugWireframeVoxelIndex);
	voxelInit(engine->debugWireframeVoxel, &vk->voxelPipeline, cubeVoxelPath, debugWireframeVoxelIndex);
	voxelUseWireframe(engine->debugWireframeVoxel);*/

	int voxelIndex1 = storageAddVoxel(&engine->storage);
	struct Voxel* voxel1 = storageGetVoxel(&engine->storage, voxelIndex1);
	voxelInit(voxel1, &vk->voxelPipeline, testVoxelPath, voxelIndex1);
	engine->activeVoxel = voxel1;

	/*int voxelIndex2 = storageAddVoxel(&engine->storage);
	struct Voxel* voxel2 = storageGetVoxel(&engine->storage, voxelIndex2);
	voxelInit(voxel2, &vk->voxelPipeline, treeVoxelPath, voxelIndex2);*/

	int i = storageAddParticle(&engine->storage);
	struct Particle* particle = storageGetParticle(&engine->storage, i);
	particleInit(particle, &vk->particlesPipeline);

	return engine;
}

void engineDestroy(struct Engine* engine) {
	storageDestroy(&engine->storage);
	animationEngineDestroy(&engine->animEngine);
	free(engine);
}

void engineSyncedUpdate(struct Engine* engine, struct VulkanDevice* vk, struct Window* window) {
	struct timespec ts;
	int success = timespec_get(&ts, TIME_UTC);
	engine->delta = (int)(ts.tv_sec - engine->lastFrameTime.tv_sec) * 1000000 + (int)(ts.tv_nsec - engine->lastFrameTime.tv_nsec) / 1000;
	engine->lastFrameTime = ts;
	handleKeyboardSynced(engine, vk, &window->keyboard);
	vulkanDeviceWaitFrame(vk);
}

void engineUpdate(struct Engine* engine, struct VulkanDevice* vk, struct Window* window) {
	handleKeyboard(engine, vk, &window->keyboard);
	handleMouse(engine, vk, &window->mouse);

	engineUpdateDebugVoxel(engine);

	storageUpdate(&engine->storage, vk, engine->delta);
	animationEngineUpdate(&engine->animEngine, engine->delta);

	if (engine->activeVoxel && engine->setCameraOnActiveObject) {
		viewSetCameraPos(&vk->view, &engine->activeVoxel->transform.position);
	}
	viewSetCameraZ(&vk->view, window->mouse.dz);
	viewUpdate(&vk->view);

	vulkanDeviceUpdateBegin(vk);
	storageCompute(&engine->storage, vk);
	vulkanDeviceUpdateEnd(vk);
}

void engineRender(struct Engine* engine, struct VulkanDevice* vk) {
	vulkanDeviceRenderBegin(vk);
	storageRender(&engine->storage, vk);
	vulkanDeviceRenderEnd(vk);
}

void enginePresent(struct Engine* engine, struct VulkanDevice* vulkanDevice) {
	vulkanDevicePresentFrame(vulkanDevice);
}
