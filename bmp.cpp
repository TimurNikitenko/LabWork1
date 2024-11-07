#include "bmp.hpp"
#include <iostream>
#include <fstream>

bool loadBMP(const std::string &filename, BMPHeader &bmpHeader, BPMInfoHeader &bmpInfoHeader, std::vector<uint8_t> &imageData) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "File opening error" << std::endl;
		return false;
	};

	file.read(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));

	if  (bmpHeader.fileType != 0x4D42) {
		std::cerr << "Not a BMP file" << std::endl;
		return false;
	};

	file.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

	imageData.resize(bmpInfoHeader.width * bmp.InfoHeader.height * 3);

	file.seekg(bmpHeader.offsetData, std::ios::beg);

	file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());

	file.close();

	return true;
}

