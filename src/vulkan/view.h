#ifndef VIEW_H
#define VIEW_H

#include "vulkan_includes.h"
#include "host_buffer.h"
#include "math/mat4.h"
#include "math/vec3.h"
#include "math/quat.h"

struct Window;

struct ProjectionBuffer {
	struct mat4 matrix;
};

struct View {
	VkDescriptorPool descriptorPool;
	VkDescriptorSetLayout projectionSetLayout;
	VkDescriptorSet projectionDescriptorSets[IMAGE_COUNT];

	struct HostBuffer hostBuffer;

	struct mat4 perspective;
	struct mat4 camera;

	struct vec3 relativeEye;
	struct vec3 center;

	struct quat rotation;
	float rx;
	float ry;
	float cameraZ;
};

void viewInit(struct View* view, struct VulkanDevice* vk, struct Window* window);
void viewDestroy(struct View* view, struct VulkanDevice* vk);

void viewBind(struct View* view, struct VulkanDevice* vk, VkCommandBuffer commandBuffer);
void viewBufferUpdate(struct View* view, int bufferWriteIndex);

void viewSetCameraZ(struct View* view, float z);
void viewSetCameraPos(struct View* view, struct vec3* pos);
void viewRotate(struct View* view, float dx, float dy);
void viewUpdate(struct View* view);

#endif // VIEW_H
