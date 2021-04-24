#include "util/ktx.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

static const uint8_t identifier[] = { 0xAB, 0x4B, 0x54, 0x58, 0x20, 0x31, 0x31, 0xBB, 0x0D, 0x0A, 0x1A, 0x0A };

struct KtxHeader readKtx(uint8_t* dest, const char* file, uint32_t maxSize) {
	struct KtxHeader header;

	FILE* fp = fopen(file, "rb");
	assert(fp);
	fread(&header, 1, sizeof(struct KtxHeader), fp);
	assert(memcmp(header.identifier, identifier, sizeof(identifier)) == 0);
	assert(header.endianness == 0x04030201);
	assert(header.bytesOfKeyValueData == 0);
	assert(header.imageSize < maxSize);
	fread(dest, 1, header.imageSize, fp);
	fclose(fp);

	return header;
}
