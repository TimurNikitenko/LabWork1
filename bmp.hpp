#ifndef BMP_HPP
#define BMP_CPP

#include <cstdint>
#include <string>
#include <vector>

#pragma pack(push, 1)

struct BMPHeader {
	uint16_t fileType{0x4D42};
	uint32_t fileSize{0};
	uint32_t reserved{0};
	uint32_t offsetDate{0};
};

struct BMPInfoHeader {
	uint32_t size{0};
	int32_t width{0};
	int32_t height{0};
	uint16_t planes{1};
	uint16_t bitCount{24};
	uint32_t compression{0};
	uint32_t sizeImage{0};
	int32_t xPixelsPerMeter{0};
	int32_t yPixelsPerMeter{0};
	uint32_t colorsUsed{0};
	uint32_t colorsImportant{0};
};

#pragma pack(pop)

bool loadBMP(const std::string &filename, BMPHeader &bmpHeader, BMPInfoHeader &bmpInfoHeader, std::vector<uint8_t> &imageData);

#endif

