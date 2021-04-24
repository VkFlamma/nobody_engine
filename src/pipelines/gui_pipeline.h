#ifndef GUI_PIPELINE_H
#define GUI_PIPELINE_H

#include "vulkan/vulkan_includes.h"
#include "vulkan/host_buffer.h"
#include "containers/vector.h"

#define GUI_PIPELINES_COUNT 3
#define MAX_GUI_OBJECTS 1024

#define GUI_VERTEX_COUNT 4

enum GuiSpecialization {
	GUI_WINDOW,
	GUI_FONT,
	GUI_BUTTON,
};

struct GuiVertex {
	float x;
	float y;
	float sizex;
	float sizey;
	uint32_t index;
};

struct GuiPushConst {
	uint32_t index;
};

struct GuiPipeline {
	struct VulkanDevice* vk;
	VkPipeline pipelines[GUI_PIPELINES_COUNT];
	VkPipelineLayout layout;

	struct HostBuffer vertexBuffer;

	struct Vector renderContexts;

	int queueIndex;
};

void guiPipelineInit(struct GuiPipeline* pipeline, struct VulkanDevice* vk);
void guiPipelineDestroy(struct GuiPipeline* pipeline);

void guiPipelineReload(struct GuiPipeline* pipeline);

struct RenderContext* guiPipelineGetRenderContext(struct GuiPipeline* pipeline);

void guiPipelineRender(struct GuiPipeline* pipeline);

void guiPipelineAddToQueue(struct GuiPipeline* pipeline, void* vertexData);

#endif // GUI_PIPELINE_H
