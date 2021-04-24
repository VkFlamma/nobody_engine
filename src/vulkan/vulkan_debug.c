#include "vulkan_debug.h"

#include <stdlib.h>

#include "util/defines.h"

VkDebugUtilsMessengerEXT callback;

char* ignored[] = {
	"Device Extension:",
	"Loading layer library",
	"Unloading layer library",
	"Inserted device layer",
	"loader_add_implicit_layer",
	"vkGetPhysicalDeviceQueueFamilyProperties2KHR() is called with non-NULL pQueueFamilyProperties before obtaining pQueueFamilyPropertyCount. It is recommended to first call vkGetPhysicalDeviceQueueFamilyProperties2KHR() with NULL pQueueFamilyProperties in order to obtain the maximal pQueueFamilyPropertyCount.",
	"vkCreateSwapchainKHR() called before getting surface present mode(s) from vkGetPhysicalDeviceSurfacePresentModesKHR().",
};

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,	void* pUserData) {
	for (int i = 0; i < ARRAY_SIZE(ignored); ++i) {
		if (strstr(pCallbackData->pMessage, ignored[i]) != NULL) {
			return VK_FALSE;
		}
	}
	printf("%s\n\n", pCallbackData->pMessage);

 	return VK_FALSE;
}

void setupDebugCallback(VkInstance instance) {
#ifdef USE_VALIDATION_LAYERS
	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	debugCreateInfo.pNext = NULL;
	debugCreateInfo.flags = 0;
	debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
								      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
									  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
								  VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT /*|
								  VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT*/;
	debugCreateInfo.pfnUserCallback = debugCallback;
	debugCreateInfo.pUserData = NULL;

	vkCreateDebugUtilsMessengerEXT(instance, &debugCreateInfo, NULL, &callback);
#endif
}

void destroyDebugCallback(VkInstance instance) {
#ifdef USE_VALIDATION_LAYERS
	vkDestroyDebugUtilsMessengerEXT(instance, callback, NULL);
#endif
}

char* errorString(VkResult errorCode) {
	switch (errorCode) {
#define STR(r) \
  case VK_##r: \
    return #r
		STR(NOT_READY);
		STR(TIMEOUT);
		STR(EVENT_SET);
		STR(EVENT_RESET);
		STR(INCOMPLETE);
		STR(ERROR_OUT_OF_HOST_MEMORY);
		STR(ERROR_OUT_OF_DEVICE_MEMORY);
		STR(ERROR_INITIALIZATION_FAILED);
		STR(ERROR_DEVICE_LOST);
		STR(ERROR_MEMORY_MAP_FAILED);
		STR(ERROR_LAYER_NOT_PRESENT);
		STR(ERROR_EXTENSION_NOT_PRESENT);
		STR(ERROR_FEATURE_NOT_PRESENT);
		STR(ERROR_INCOMPATIBLE_DRIVER);
		STR(ERROR_TOO_MANY_OBJECTS);
		STR(ERROR_FORMAT_NOT_SUPPORTED);
		STR(ERROR_SURFACE_LOST_KHR);
		STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
		STR(SUBOPTIMAL_KHR);
		STR(ERROR_OUT_OF_DATE_KHR);
		STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
		STR(ERROR_VALIDATION_FAILED_EXT);
		STR(ERROR_INVALID_SHADER_NV);
		STR(ERROR_OUT_OF_POOL_MEMORY);
#undef STR
	default:
		return "UNKNOWN_ERROR";
	}
}
