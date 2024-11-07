#ifndef BMP_HPP
#define BMP_HPP

#include <cstdint>
#include <string>
#include <vector>

struct BMPHeader {
	uint16_t fileType;
	uint32_t fileSize;
	uint32_t reserved1;
	uint32_t reserved2;
	uint32_t offsetData;
};

struct BMPInfoHeader {
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t sizeImage;
	int32_t xPixelsPerMeter;
	int32_t yPixelsPerMeter;
	uint32_t colorsUsed;
	uint32_t colorsImportant;
};

class BMPImage {
	public:
		BMPHeader header;
		BMPInfoHeader infoHeader;
		std::vector<uint8_t> pixelData;

		void load(const std::string &filename);
		void save(const std::string &filename);
		void rotateClockwise();
		void rotateCounterclockwise();
		void applyGaussianFilter();
};

#endif

