#ifndef KTX_H
#define KTX_H

#include <stdint.h>

struct KtxHeader {
	uint8_t identifier[12];
	uint32_t endianness;
	uint32_t glType;
	uint32_t glTypeSize;
	uint32_t glFormat;
	uint32_t glInternalFormat;
	uint32_t glBaseInternalFormat;
	uint32_t pixelWidth;
	uint32_t pixelHeight;
	uint32_t pixelDepth;
	uint32_t numberOfArrayElements;
	uint32_t numberOfFaces;
	uint32_t numberOfMipmapLevels;
	uint32_t bytesOfKeyValueData;
	uint32_t imageSize;
};

struct KtxHeader readKtx(uint8_t* dest, const char* file, uint32_t maxSize);

#endif // KTX_H
