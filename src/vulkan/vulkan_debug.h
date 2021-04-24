#ifndef VULKAN_DEBUG_H
#define VULKAN_DEBUG_H

#include <assert.h>
#include <stdio.h>

#include "vulkan_includes.h"

static const char* validationLayers[1] = {
	"VK_LAYER_KHRONOS_validation",
};

static VkValidationFeatureEnableEXT validationFeaturesEnable[] = {
	VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
	VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
	VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
	// VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT, // cannot be enabled together with VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT
	VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
};

static VkValidationFeatureDisableEXT validationFeaturesDisable[] = {
	VK_VALIDATION_FEATURE_DISABLE_ALL_EXT,
};

void setupDebugCallback(VkInstance instance);
void destroyDebugCallback(VkInstance instance);

char* errorString(VkResult errorCode);

#define VK_ASSERT(f)														\
	{																		\
		VkResult result = (f);												\
		if (result != VK_SUCCESS) {											\
			printf("%s %s %i\n", errorString(result), __FILE__, __LINE__);	\
			assert(result == VK_SUCCESS);									\
		}																	\
	}

#endif // VULKAN_DEBUG_H
