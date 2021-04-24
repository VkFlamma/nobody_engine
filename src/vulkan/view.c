#include "view.h"

#include "vulkan_device.h"
#include "window/window.h"
#include "math/math_util.h"
#include "math/quat.h"
#include "math/vec4.h"
#include "vulkan_debug.h"

void viewCreateDescriptorSet(struct View* view, struct VulkanDevice* vk) {
	VkDescriptorPoolSize descriptorPoolSize;
	descriptorPoolSize.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	descriptorPoolSize.descriptorCount = 1;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.pNext = NULL;
	descriptorPoolCreateInfo.flags = 0;
	descriptorPoolCreateInfo.maxSets = IMAGE_COUNT;
	descriptorPoolCreateInfo.poolSizeCount = 1;
	descriptorPoolCreateInfo.pPoolSizes = &descriptorPoolSize;
	vkCreateDescriptorPool(vk->device, &descriptorPoolCreateInfo, NULL, &view->descriptorPool);

	VkDescriptorSetLayoutBinding projectionBinding;
	projectionBinding.binding = 0;
	projectionBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	projectionBinding.descriptorCount = 1;
	projectionBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	projectionBinding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.pNext = NULL;
	descriptorSetLayoutCreateInfo.flags = 0;
	descriptorSetLayoutCreateInfo.bindingCount = 1;
	descriptorSetLayoutCreateInfo.pBindings = &projectionBinding;

	vkCreateDescriptorSetLayout(vk->device, &descriptorSetLayoutCreateInfo, NULL, &view->projectionSetLayout);

	VkDescriptorSetLayout layouts[IMAGE_COUNT];
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		layouts[i] = view->projectionSetLayout;
	}

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo;
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pNext = NULL;
	descriptorSetAllocateInfo.descriptorPool = view->descriptorPool;
	descriptorSetAllocateInfo.descriptorSetCount = IMAGE_COUNT;
	descriptorSetAllocateInfo.pSetLayouts = layouts;

	VK_ASSERT(vkAllocateDescriptorSets(vk->device, &descriptorSetAllocateInfo, view->projectionDescriptorSets));
}

void viewUpdateDescriptorSet(struct View* view, struct VulkanDevice* vk) {
	for (int i = 0; i < IMAGE_COUNT; ++i) {
		VkDescriptorBufferInfo bufferInfo;
		bufferInfo.buffer = view->hostBuffer.buffer;
		bufferInfo.offset = view->hostBuffer.size * i;
		bufferInfo.range = view->hostBuffer.size;

		VkWriteDescriptorSet projectionWrite;
		projectionWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		projectionWrite.pNext = NULL;
		projectionWrite.dstSet = view->projectionDescriptorSets[i];
		projectionWrite.dstBinding = 0;
		projectionWrite.dstArrayElement = 0;
		projectionWrite.descriptorCount = 1;
		projectionWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		projectionWrite.pImageInfo = NULL;
		projectionWrite.pBufferInfo = &bufferInfo;
		projectionWrite.pTexelBufferView = NULL;

		vkUpdateDescriptorSets(vk->device, 1, &projectionWrite, 0, NULL);
	}
}

void viewUpdateProjection(struct View* view, int window_width, int window_height) {
	mat4_zero(&view->perspective);
	float aspect = (float)window_width / (float)window_height;
	float fovy = 60.0f;
	float fovyRadians = radians(fovy);
	float zNear = 0.1f;
	float zFar = 1000.0f;
	float tanHalfFovy = (float)tan(fovyRadians * 0.5);

	view->perspective.m[0] = 1.0f / (aspect * tanHalfFovy);
	view->perspective.m[5] = 1.0f / tanHalfFovy;
	view->perspective.m[10] = -(zFar + zNear) / (zFar - zNear);
	view->perspective.m[11] = -1.0f;
	view->perspective.m[14] = -(2.0f * zFar * zNear) / (zFar - zNear);

	mat4_identity(&view->camera);
}

void lookAt(struct mat4* dest, struct vec3* eye, struct vec3* center, struct vec3* up) {
	struct vec3 f, u, s;

	vec3_sub(&f, center, eye);
	vec3_normalize(&f);

	vec3_cross(&s, &f, up);
	vec3_normalize(&s);
	vec3_cross(&u, &s, &f);

	dest->m[0] = s.x;
	dest->m[1] = u.x;
	dest->m[2] = -f.x;
	dest->m[3] = 0.0f;
	dest->m[4] = s.y;
	dest->m[5] = u.y;
	dest->m[6] = -f.y;
	dest->m[7] = 0.0f;
	dest->m[8] = s.z;
	dest->m[9] = u.z;
	dest->m[10] = -f.z;
	dest->m[11] = 0.0f;
	dest->m[12] = -vec3_dot(&s, eye);
	dest->m[13] = -vec3_dot(&u, eye);
	dest->m[14] = vec3_dot(&f, eye);
	dest->m[15] = 1.0f;
}


void viewInit(struct View* view, struct VulkanDevice* vk, struct Window* window) {
	vec3_init(&view->relativeEye, 0.0f, 0.0f, 15.0f);
	vec3_init(&view->center, 0.0f, 0.0f, 0.0f);
	quat_init(&view->rotation, 0.0f, 0.0f, 0.0f, 1.0f);
	view->rx = 0.0f;
	view->ry = 0.0f;
	view->cameraZ = MIN_CAMERA_Z;

	hostBufferInit(&view->hostBuffer, vk, sizeof(struct ProjectionBuffer), 1, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
	viewUpdateProjection(view, window->width, window->height);
	viewCreateDescriptorSet(view, vk);
	viewUpdateDescriptorSet(view, vk);
}

void viewDestroy(struct View* view, struct VulkanDevice* vk) {
	hostBufferDestroy(&view->hostBuffer, vk);
	vkDestroyDescriptorSetLayout(vk->device, view->projectionSetLayout, NULL);
	vkDestroyDescriptorPool(vk->device, view->descriptorPool, NULL);
}

void viewBind(struct View* view, struct VulkanDevice* vk, VkCommandBuffer commandBuffer) {
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vk->voxelPipeline.layout, 0, 1, &view->projectionDescriptorSets[vk->bufferReadIndex], 0, NULL);
}

void viewBufferUpdate(struct View* view, int bufferWriteIndex) {
	struct vec3 up;
	vec3_init(&up, 0.0f, 1.0f, 0.0f);
	struct vec3 eye;
	vec3_add(&eye, &view->center, &view->relativeEye);
	lookAt(&view->camera, &eye, &view->center, &up);

	struct mat4 PV;
	mat4_mul(&PV, &view->perspective, &view->camera);
	hostBufferWrite(&view->hostBuffer, bufferWriteIndex, 0, 1, &PV);
}

void viewSetCameraZ(struct View* view, float z) {
	view->cameraZ += -z * CAMERA_SCROLL_MODIFIER;
	if (view->cameraZ < MIN_CAMERA_Z) {
		view->cameraZ = MIN_CAMERA_Z;
	} else if (view->cameraZ > MAX_CAMERA_Z) {
		view->cameraZ = MAX_CAMERA_Z;
	}
}

void viewSetCameraPos(struct View* view, struct vec3* pos) {
	view->center = *pos;
}

void viewRotate(struct View* view, float dx, float dy) {
	view->rx += dx * MOUSE_SPEED;
	view->ry += dy * MOUSE_SPEED;

	view->ry = view->ry < 60.0f ? view->ry : 60.0f;
	view->ry = view->ry > -60.0f ? view->ry : -60.0f;

	float halfAngleX = radians(view->rx) * 0.5f;
	float halfAngleY = radians(-view->ry) * 0.5f;

	struct quat rx, ry;
	rx.x = (float)sin(halfAngleY);
	rx.y = 0.0f;
	rx.z = 0.0f;
	rx.w = (float)cos(halfAngleY);
	ry.x = 0.0f;
	ry.y = (float)sin(halfAngleX);
	ry.z = 0.0f;
	ry.w = (float)cos(halfAngleX);
	quat_normalize(&rx);
	quat_normalize(&ry);

	quat_mul(&view->rotation, &rx, &ry);
	quat_normalize(&view->rotation);
}

void viewUpdate(struct View* view) {
	struct mat4 rotationMatrix;
	quat_get_matrix(&view->rotation, &rotationMatrix);
	struct vec4 startPoint;
	vec4_init(&startPoint, 0.0f, 0.0f, view->cameraZ, 1.0f);
	struct vec4 rotationPoint;
	mat4_mul_vec(&rotationPoint, &rotationMatrix, &startPoint);

	view->relativeEye.x = rotationPoint.x;
	view->relativeEye.y = rotationPoint.y;
	view->relativeEye.z = rotationPoint.z;
}
