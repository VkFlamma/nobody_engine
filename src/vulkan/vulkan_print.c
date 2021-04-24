#include "vulkan_print.h"

#include <stdio.h>

#include "vulkan_device.h"
#include "vulkan_debug.h"

void printInstanceExtensions(struct VulkanDevice* vk) {
#ifdef VULKAN_PRINT_INFO
	uint32_t propertyCount;
	VK_ASSERT(vkEnumerateInstanceExtensionProperties(NULL, &propertyCount, NULL));
	VkExtensionProperties* properties = getAllocationBuffer(vk, sizeof(VkExtensionProperties) * propertyCount);
	VK_ASSERT(vkEnumerateInstanceExtensionProperties(NULL, &propertyCount, vk->allocationBuffer));
	printf("\nInstance extensions:\n");
	for (uint32_t i = 0; i < propertyCount; ++i) {
		printf("%s %u\n", properties[i].extensionName, properties[i].specVersion);
	}
	printf("\n");
#endif
}

void printPhysicalDeviceInfo(struct VulkanDevice* vk) {
#ifdef VULKAN_PRINT_INFO
	VkPhysicalDeviceProperties* props = &vk->physicalDevice.properties;
	printf("Selected physical device:\n");
	printf("Name: %s\n", props->deviceName);
	printf("Api version: %u\n", props->apiVersion);
	printf("Driver version: %u\n", props->driverVersion);
	printf("Vendor ID: %u\n", props->vendorID);
	printf("Device ID: %u\n", props->deviceID);
	switch (props->deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			printf("Device type: Other\n");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			printf("Device type: Integrated GPU\n");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			printf("Device type: Discrete GPU\n");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			printf("Device type: Virtual GPU\n");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			printf("Device type: Cpu\n");
			break;
		default:
			break;
	}
	printf("\n");
#endif
}

void printDeviceExtensions(struct VulkanDevice* vk) {
#ifdef VULKAN_PRINT_INFO
	uint32_t propertyCount;
	vkEnumerateDeviceExtensionProperties(vk->physicalDevice.physicalDevice, NULL, &propertyCount, NULL);
	VkExtensionProperties* properties = getAllocationBuffer(vk, sizeof(VkExtensionProperties) * propertyCount);
	vkEnumerateDeviceExtensionProperties(vk->physicalDevice.physicalDevice, NULL, &propertyCount, properties);
	printf("\nDevice extensions:\n");
	for (uint32_t i = 0; i < propertyCount; ++i) {
		printf("%s %u\n", properties[i].extensionName, properties[i].specVersion);
	}
	printf("\n");
#endif
}

void printDriverInfo(VkPhysicalDeviceDriverProperties* driverProperties) {
#ifdef VULKAN_PRINT_INFO
	printf("Driver properties:\n");
	printf("ID: %i\n", driverProperties->driverID);
	printf("Name: %s\n", driverProperties->driverName);
	printf("Info: %s\n", driverProperties->driverInfo);
	printf("Conformance version: %i.%i.%i.%i\n", driverProperties->conformanceVersion.major, driverProperties->conformanceVersion.minor, driverProperties->conformanceVersion.subminor, driverProperties->conformanceVersion.patch);
	printf("\n");
#endif
}

void printPCIBusInfo(VkPhysicalDevicePCIBusInfoPropertiesEXT* pciBusProperties) {
#ifdef VULKAN_PRINT_INFO
	printf("PCI bus properties:\n");
	printf("%u", pciBusProperties->pciDomain);
	printf("%u", pciBusProperties->pciBus);
	printf("%u", pciBusProperties->pciDevice);
	printf("%u", pciBusProperties->pciFunction);
	printf("\n");
#endif
}
