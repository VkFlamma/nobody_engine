#include "memory_block.h"

#include <assert.h>

struct MemoryBlock* findBlock(struct Vector* blocks, VkDeviceSize size, VkDeviceSize alignment) {
	for (int i = 0; i < blocks->count; ++i) {
		struct MemoryBlock* block = vectorGet(blocks, i);
		VkDeviceSize alignmentOffset = (alignment - block->offset) % alignment;
		if (block->free && block->size >= size + alignmentOffset) {
			struct MemoryBlock* remaining = NULL;
			if (block->size > size + alignmentOffset) {
				remaining = vectorAddAndGet(blocks);
				remaining->memory = block->memory;
				remaining->offset = block->offset + size + alignmentOffset;
				assert(block->size > size);
				remaining->size = block->size - size;
				if (block->mapped != NULL) {
					remaining->mapped = block->mapped + size + alignmentOffset;
				}
				else {
					remaining->mapped = block->mapped;
				}
				remaining->free = true;
				block->size = size;
			}
			if (alignmentOffset > 0) {
				struct MemoryBlock* prevBlock = vectorGet(blocks, i - 1);
				prevBlock->size += alignmentOffset;
			}
			block->free = false;
			return block;
		}
	}
	return NULL;
}

void freeBlock(struct Vector* blocks, struct MemoryBlock* block) {
	for (int i = 0; i < blocks->count; ++i) {
		struct MemoryBlock* currentBlock = vectorGet(blocks, i);
		if (currentBlock->offset == block->offset) {
			currentBlock->free = true;
			int j = i;
			while (j > 0 && ((struct MemoryBlock*)vectorGet(blocks, j - 1))->free) {
				--j;
			}
			while (i < blocks->count - 1 && ((struct MemoryBlock*)vectorGet(blocks, i + 1))->free) {
				++i;
			}
			if (i != j) {
				struct MemoryBlock* startBlock = vectorGet(blocks, j);
				for (int k = j + 1; k <= i; ++k) {
					struct MemoryBlock* emptyBlock = vectorGet(blocks, k);
					startBlock->size += emptyBlock->size;
				}
				vectorFreeRange(blocks, j + 1, i);
			}
			return;
		}
	}
	assert(0);
}
