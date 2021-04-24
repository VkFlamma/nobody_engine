#ifndef VULKAN_PRINT_H
#define VULKAN_PRINT_H

#include "vulkan_includes.h"

struct VulkanDevice;

void printInstanceExtensions(struct VulkanDevice* vk);
void printPhysicalDeviceInfo(struct VulkanDevice* vk);
void printDeviceExtensions(struct VulkanDevice* vk);
void printDriverInfo(VkPhysicalDeviceDriverProperties* driverProperties);
void printPCIBusInfo(VkPhysicalDevicePCIBusInfoPropertiesEXT* pciBusProperties);

#endif // VULKAN_PRINT_H
