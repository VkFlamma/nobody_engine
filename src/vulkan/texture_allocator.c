#include "texture_allocator.h"

#include "vulkan_device.h"
#include "vulkan_debug.h"
#include "util/defines.h"

void createDescriptorPoolAndSet(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk) {
    VkDescriptorPoolSize poolSize;
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = MAX_TEXTURES;

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;
    descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptorPoolCreateInfo.pNext = NULL;
    descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    descriptorPoolCreateInfo.maxSets = IMAGE_COUNT;
    descriptorPoolCreateInfo.poolSizeCount = 1;
    descriptorPoolCreateInfo.pPoolSizes = &poolSize;

    VK_ASSERT(vkCreateDescriptorPool(vk->device, &descriptorPoolCreateInfo, NULL, &textureAllocator->descriptorPool));

    VkDescriptorSetLayoutBinding textureBinding;
    textureBinding.binding = 0;
    textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    textureBinding.descriptorCount = 1;
    textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
    textureBinding.pImmutableSamplers = NULL;

    VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo;
    descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptorSetLayoutCreateInfo.pNext = NULL;
    descriptorSetLayoutCreateInfo.flags = 0;
    descriptorSetLayoutCreateInfo.bindingCount = 1;
    descriptorSetLayoutCreateInfo.pBindings = &textureBinding;

    VK_ASSERT(vkCreateDescriptorSetLayout(vk->device, &descriptorSetLayoutCreateInfo, NULL, &textureAllocator->descriptorSetLayout));
}

VkMemoryRequirements getImageMemoryRequirements(struct VulkanDevice* vk, VkImage image) {
    VkImageMemoryRequirementsInfo2 imageMemoryRequirementsInfo;
    imageMemoryRequirementsInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2;
    imageMemoryRequirementsInfo.pNext = NULL;
    imageMemoryRequirementsInfo.image = image;

    VkMemoryRequirements2 memoryRequirements;
    memoryRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2;
    memoryRequirements.pNext = NULL;
    vkGetImageMemoryRequirements2(vk->device, &imageMemoryRequirementsInfo, &memoryRequirements);
    return memoryRequirements.memoryRequirements;
}

void getMemoryIndex(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk) {
    VkImageCreateInfo imageCreateInfo;
    imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateInfo.pNext = NULL;
    imageCreateInfo.flags = 0;
    imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
    imageCreateInfo.format = VK_FORMAT_BC7_UNORM_BLOCK;
    imageCreateInfo.extent.width = 1;
    imageCreateInfo.extent.height = 1;
    imageCreateInfo.extent.depth = 1;
    imageCreateInfo.mipLevels = 1;
    imageCreateInfo.arrayLayers = 1;
    imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageCreateInfo.usage = textureAllocator->imageUsage;
    imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageCreateInfo.queueFamilyIndexCount = 0;
    imageCreateInfo.pQueueFamilyIndices = NULL;
    imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    VkImage dummyImage;
    VK_ASSERT(vkCreateImage(vk->device, &imageCreateInfo, NULL, &dummyImage));
    VkMemoryRequirements memoryRequirements = getImageMemoryRequirements(vk, dummyImage);
    vkDestroyImage(vk->device, dummyImage, NULL);

    textureAllocator->memoryType = findMemoryType(vk, memoryRequirements.memoryTypeBits, textureAllocator->memoryProperties);
    assert(textureAllocator->memoryType != UINT32_MAX);
}

void allocateTextureMemory(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk) {
    getMemoryIndex(textureAllocator, vk);

    VkMemoryPriorityAllocateInfoEXT memoryPriority;
    memoryPriority.sType = VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT;
    memoryPriority.pNext = NULL;
    memoryPriority.priority = 1.0f;

    VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo;
    memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
    memoryAllocateFlagsInfo.pNext = &memoryPriority;
    memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT;
    memoryAllocateFlagsInfo.deviceMask = 0;

    VkMemoryAllocateInfo memoryAllocateInfo;
    memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
    memoryAllocateInfo.allocationSize = textureAllocator->size;
    memoryAllocateInfo.memoryTypeIndex = textureAllocator->memoryType;

    VK_ASSERT(vkAllocateMemory(vk->device, &memoryAllocateInfo, NULL, &textureAllocator->memory));
}

void textureAllocatorInit(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk) {
    textureAllocator->imageUsage = VK_IMAGE_USAGE_SAMPLED_BIT;
    textureAllocator->memoryProperties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    textureAllocator->size = TEXTURE_MEMORY_ALLOCATION_SIZE;
    createDescriptorPoolAndSet(textureAllocator, vk);
    allocateTextureMemory(textureAllocator, vk);

    vectorInit(&textureAllocator->blocks, 256, sizeof(struct MemoryBlock));
    struct MemoryBlock* block = vectorAddAndGet(&textureAllocator->blocks);
    block->memory = textureAllocator->memory;
    block->offset = 0;
    block->size = textureAllocator->size;
    block->mapped = NULL;
    block->free = true;
}

void textureAllocatorDestroy(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk) {
    vkDestroyDescriptorSetLayout(vk->device, textureAllocator->descriptorSetLayout, NULL);
    vkDestroyDescriptorPool(vk->device, textureAllocator->descriptorPool, NULL);
    vkFreeMemory(vk->device, textureAllocator->memory, NULL);
}

void bindImageMemory(struct VulkanDevice* vk, VkImage image, VkDeviceMemory memory, VkDeviceSize offset) {
    VkBindImageMemoryInfo bindImageMemoryInfo;
    bindImageMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
    bindImageMemoryInfo.pNext = NULL;
    bindImageMemoryInfo.image = image;
    bindImageMemoryInfo.memory = memory;
    bindImageMemoryInfo.memoryOffset = offset;
    VK_ASSERT(vkBindImageMemory2(vk->device, 1, &bindImageMemoryInfo));
}

struct MemoryBlock requestImageMemory(struct TextureAllocator* textureAllocator, struct VulkanDevice* vk, VkImage image) {
    struct MemoryBlock* block = NULL;
    VkMemoryRequirements memoryRequirements = getImageMemoryRequirements(vk, image);
    uint32_t memoryType = findMemoryType(vk, memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (textureAllocator->memoryType == memoryType) {
        block = findBlock(&textureAllocator->blocks, memoryRequirements.size, memoryRequirements.alignment);
    }
#ifdef MEMORY_CHECKS
    assert(block);
#endif
    bindImageMemory(vk, image, block->memory, block->offset);
    return *block;
}

void freeImageMemory(struct TextureAllocator* textureAllocator, struct MemoryBlock* block) {
    if (textureAllocator->memoryType) {
        freeBlock(&textureAllocator->blocks, block);
    } else {
#ifdef MEMORY_CHECKS
        assert(0);
#endif
    }
}
