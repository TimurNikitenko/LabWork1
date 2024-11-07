/* Timur Nikitenko st128375@student.spbu.ru
 *first labwork
 */


#include "bmp.hpp"
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cmath>

void BMPImage::load(const std::string &filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "File opening error" << std::endl;
		return false;
	};

	file.read(reinterpret_cast<char*>&(header), sizeof(header));

	if  (header.fileType != 0x4D42) {
		std::cerr << "Not a BMP file" << std::endl;
		return false;
	};

	file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

	pixelData.resize(infoHeader.sizeImage);

	file.seekg(header.offsetData, std::ios::beg);

	file.read(reinterpret_cast<char*>(pixelData.data()), pixelData.size());

	file.close();
}

void BMPImage::save(const std::string &filename) {
	std::ofstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Error when opening file for saving" << std::endl;
		return false;
	};
	file.write(reinterpret_cast<char*>(&header), sizeof(header));
	file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));  
	file.write(reinterpret_cast<char*>(pixelData.data()), pixelData.size());
	file.close();
}

void BMPImage::rotateClockwise() {
    std::vector<uint8_t> newPixelData(infoHeader.sizeImage);
    int newWidth = infoHeader.height;
    int newHeight = infoHeader.width;

    for (int i = 0; i < infoHeader.width; i++) {
        for (int j = 0; j < infoHeader.height; j++) {
            int oldIndex = (j * infoHeader.width + i) * (infoHeader.bitCount / 8);
            int newIndex = ((newHeight - 1 - i) * newWidth + j) * (infoHeader.bitCount / 8);
            newPixelData[newIndex] = pixelData[oldIndex];
            newPixelData[newIndex + 1] = pixelData[oldIndex + 1];
            newPixelData[newIndex + 2] = pixelData[oldIndex + 2];
            if (infoHeader.bitCount == 32) {
                newPixelData[newIndex + 3] = pixelData[oldIndex + 3];
            }
        }
    }

  
    std::swap(infoHeader.width, infoHeader.height);
    infoHeader.sizeImage = newWidth * newHeight * (infoHeader.bitCount / 8);
    header.fileSize = sizeof(header) + sizeof(infoHeader) + infoHeader.sizeImage;

    pixelData = std::move(newPixelData);
}

void BMPImage::rotateCounterclockwise() {
    std::vector<uint8_t> newPixelData(infoHeader.sizeImage);
    int newWidth = infoHeader.height;
    int newHeight = infoHeader.width;

    for (int i = 0; i < infoHeader.width; i++) {
        for (int j = 0; j < infoHeader.height; j++) {
            int oldIndex = (j * infoHeader.width + i) * (infoHeader.bitCount / 8);
            int newIndex = (i * newWidth + (newHeight - 1 - j)) * (infoHeader.bitCount / 8);
            newPixelData[newIndex] = pixelData[oldIndex];
            newPixelData[newIndex + 1] = pixelData[oldIndex + 1];
            newPixelData[newIndex + 2] = pixelData[oldIndex + 2];
            if (infoHeader.bitCount == 32) {
                newPixelData[newIndex + 3] = pixelData[oldIndex + 3]; 
            }
        }
    }

    std::swap(infoHeader.width, infoHeader.height);
    infoHeader.sizeImage = newWidth * newHeight * (infoHeader.bitCount / 8);
    header.fileSize = sizeof(header) + sizeof(infoHeader) + infoHeader.sizeImage;

    pixelData = std::move(newPixelData);
}


void BMPImage::applyGaussianFilter() {
    std::vector<uint8_t> newPixelData(pixelData.size());
    
    const double kernel[5][5] = {
        {1/273.0, 4/273.0, 6/273.0, 4/273.0, 1/273.0},
        {4/273.0, 16/273.0, 24/273.0, 16/273.0, 4/273.0},
        {6/273.0, 24/273.0, 36/273.0, 24/273.0, 6/273.0},
        {4/273.0, 16/273.0, 24/273.0, 16/273.0, 4/273.0},
        {1/273.0, 4/273.0, 6/273.0, 4/273.0, 1/273.0}
    };
    
    int kernelRadius = 2;

    for (int y = kernelRadius; y < infoHeader.height - kernelRadius; y++) {
        for (int x = kernelRadius; x < infoHeader.width - kernelRadius; x++) {
            double rTotal = 0.0, gTotal = 0.0, bTotal = 0.0;

            for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                    int currentX = x + kx;
                    int currentY = y + ky;
                    int currentIndex = (currentY * infoHeader.width + currentX) * (infoHeader.bitCount / 8);
                    
                    rTotal += pixelData[currentIndex] * kernel[ky + kernelRadius][kx + kernelRadius];
                    gTotal += pixelData[currentIndex + 1] * kernel[ky + kernelRadius][kx + kernelRadius];
                    bTotal += pixelData[currentIndex + 2] * kernel[ky + kernelRadius][kx + kernelRadius];
                }
            }

            int newIndex = (y * infoHeader.width + x) * (infoHeader.bitCount / 8);
            newPixelData[newIndex] = std::min(std::max(int(rTotal), 0), 255);
            newPixelData[newIndex + 1] = std::min(std::max(int(gTotal), 0), 255);
            newPixelData[newIndex + 2] = std::min(std::max(int(bTotal), 0), 255);
            if (infoHeader.bitCount == 32) {
                newPixelData[newIndex + 3] = pixelData[newIndex + 3];
            }
        }
    }

    pixelData = std::move(newPixelData);
}

